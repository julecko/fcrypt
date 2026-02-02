#include "util.h"
#include "crypt/file_format.h"

#include <sodium.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 4096
#define SALT_SIZE crypto_pwhash_SALTBYTES
#define KEY_SIZE crypto_secretstream_xchacha20poly1305_KEYBYTES

int encrypt(const char *input_filename, const char *password) {
    if (sodium_init() < 0) {
        fputs("libsodium init failed\n", stderr);
        return -1;
    }

    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file) {
        fputs("Encrypt: failed to open input file\n", stderr);
        return -1;
    }

    char *output_filename = make_enc_filename(input_filename);
    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        fputs("Encrypt: failed to open output file\n", stderr);
        fclose(input_file);
        return -1;
    }

    // File header
    fcrt_header_t fcrt_header = {0};

    randombytes_buf(fcrt_header.salt, sizeof(fcrt_header.salt));
    fcrt_header.version = FCRT_VERSION_1;

    // Derive key
    unsigned char key[KEY_SIZE];
    if (crypto_pwhash(key, KEY_SIZE,
                      password, strlen(password),
                      fcrt_header.salt,
                      crypto_pwhash_OPSLIMIT_INTERACTIVE,
                      crypto_pwhash_MEMLIMIT_INTERACTIVE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        fputs("Out of memory: key derivation failed\n", stderr);
        
        remove_file(output_filename);
        free(output_filename);

        fclose(input_file);
        fclose(output_file);
        return -1;
    }

    // Init secretstream
    crypto_secretstream_xchacha20poly1305_state state;
    if (crypto_secretstream_xchacha20poly1305_init_push(&state, fcrt_header.stream_header, key) != 0) {
        fputs("Stream init failed\n", stderr);

        remove_file(output_filename);
        free(output_filename);

        fclose(input_file);
        fclose(output_file);
        return -1;
    }

    int result = fcrt_write_header(output_file, &fcrt_header);
    if (result == -1) {
        fputs("Encrypt: writing header to file failed\n", stderr);

        remove_file(output_filename);
        free(output_filename);

        fclose(input_file);
        fclose(output_file);
        return -1;
    }

    // Encrypt chunks
    unsigned char in[CHUNK_SIZE];
    unsigned char out[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
    size_t rlen;
    while ((rlen = fread(in, 1, CHUNK_SIZE, input_file)) > 0) {
        unsigned char tag = feof(input_file) ? crypto_secretstream_xchacha20poly1305_TAG_FINAL : 0;

        unsigned long long out_len;
        crypto_secretstream_xchacha20poly1305_push(&state, out, &out_len, in, rlen, NULL, 0, tag);
        fwrite(out, 1, out_len, output_file);
    }

    free(output_filename);

    fclose(input_file);
    fclose(output_file);

    sodium_memzero(key, KEY_SIZE);
    return 0;
}

int decrypt(const char *input_filename, const char *password) {
    if (sodium_init() < 0) {
        fputs("libsodium init failed\n", stderr);
        return -1;
    }

    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file) {
        fputs("Decrypt: failed to open input file\n", stderr);
        return -1;
    }

    fcrt_header_t fcrt_header = {0};

    int result = fcrt_read_header(input_file, &fcrt_header);
    if (result == -1) {
        fputs("Decrypt: failed to read file header\n", stderr);
        fclose(input_file);
        return -1;
    }

    // Derive key
    unsigned char key[KEY_SIZE];
    if (crypto_pwhash(key, KEY_SIZE,
                      password, strlen(password),
                      fcrt_header.salt,
                      crypto_pwhash_OPSLIMIT_INTERACTIVE,
                      crypto_pwhash_MEMLIMIT_INTERACTIVE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        fputs("Out of memory: key derivation failed\n", stderr);
        fclose(input_file);
        return -1;
    }

    crypto_secretstream_xchacha20poly1305_state state;
    if (crypto_secretstream_xchacha20poly1305_init_pull(&state, fcrt_header.stream_header, key) != 0) {
        fputs("Decrypt: stream init failed (wrong password?)\n", stderr);
        fclose(input_file);
        sodium_memzero(key, KEY_SIZE);
        return -1;
    }

    char *output_filename = make_dec_filename(input_filename);
    FILE *output_file = fopen(output_filename, "wb");


    if (!output_file) {
        fputs("Decrypt: failed to open output file\n", stderr);
        fclose(input_file);
        sodium_memzero(key, KEY_SIZE);
        return -1;
    }

    // Decrypt chunks
    unsigned char in[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
    unsigned char out[CHUNK_SIZE];
    size_t rlen;

    while ((rlen = fread(in, 1, sizeof(in), input_file)) > 0) {
        unsigned long long out_len;
        unsigned char tag;

        if (crypto_secretstream_xchacha20poly1305_pull(&state, out, &out_len, &tag, in, rlen, NULL, 0) != 0) {
            fputs("Decrypt: file corrupted or wrong password\n", stderr);

            remove_file(output_filename);
            free(output_filename);

            fclose(input_file);
            fclose(output_file);
            sodium_memzero(key, KEY_SIZE);
            return -1;
        }

        fwrite(out, 1, out_len, output_file);

        if (tag & crypto_secretstream_xchacha20poly1305_TAG_FINAL) break;
    }

    free(output_filename);

    fclose(input_file);
    fclose(output_file);
    sodium_memzero(key, KEY_SIZE);

    return 0;
}
