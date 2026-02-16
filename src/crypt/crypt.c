#include "util.h"
#include "crypt/file_format.h"
#include "logger.h"

#include <sodium.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 4096
#define SALT_SIZE crypto_pwhash_SALTBYTES
#define KEY_SIZE crypto_secretstream_xchacha20poly1305_KEYBYTES


static void log_progress(long processed, long total) {
    static int last_logged = -1;
    if (total <= 0) {
        log_warn("Empty input");
        return;
    }

    int percent = (int)((processed * 100) / total);
    if (percent != last_logged && percent % 5 == 0) {
        log_info("Progress: %d%%", percent);
        last_logged = percent;
    }
}

int encrypt(const char *input_filename, const char *password) {
    if (sodium_init() < 0) {
        log_error("libsodium init failed");
        return -1;
    }

    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file) {
        log_error("Encrypt: failed to open input file '%s'", input_filename);
        return -1;
    }

    long total_size = get_file_size(input_file);

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
        log_error("Out of memory: key derivation failed");
        fclose(input_file);
        return -1;
    }

    // Init secretstream
    crypto_secretstream_xchacha20poly1305_state state;
    if (crypto_secretstream_xchacha20poly1305_init_push(&state, fcrt_header.stream_header, key) != 0) {
        log_error("Stream init failed");
        fclose(input_file);
        return -1;
    }

    char *output_filename = make_enc_filename(input_filename);
    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        log_error("Encrypt: failed to open output file '%s'", output_filename);
        fclose(input_file);
        free(output_filename);
        return -1;
    }

    log_info("Encrypting file '%s' -> '%s'", input_filename, output_filename);

    if (fcrt_write_header(output_file, &fcrt_header) == -1) {
        log_error("Encrypt: writing header to file failed");
        remove_file(output_filename);
        free(output_filename);

        fclose(input_file);
        fclose(output_file);
        return -1;
    }

    log_debug("Header written, starting chunk encryption...");

    unsigned char in[CHUNK_SIZE];
    unsigned char out[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
    size_t rlen;
    long processed = 0;
    while ((rlen = fread(in, 1, CHUNK_SIZE, input_file)) > 0) {
        unsigned char tag = feof(input_file) ? crypto_secretstream_xchacha20poly1305_TAG_FINAL : 0;
        unsigned long long out_len;
        crypto_secretstream_xchacha20poly1305_push(&state, out, &out_len, in, rlen, NULL, 0, tag);
        fwrite(out, 1, out_len, output_file);

        processed += rlen;
        log_progress(processed, total_size);
    }

    log_info("File encryption complete: '%s'", output_filename);

    free(output_filename);

    fclose(input_file);
    fclose(output_file);

    sodium_memzero(key, KEY_SIZE);

    return 0;
}

int decrypt(const char *input_filename, const char *password) {
    if (sodium_init() < 0) {
        log_error("libsodium init failed");
        return -1;
    }

    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file) {
        log_error("Decrypt: failed to open input file '%s'", input_filename);
        return -1;
    }

    fcrt_header_t fcrt_header = {0};
    if (fcrt_read_header(input_file, &fcrt_header) == -1) {
        log_error("Decrypt: failed to read file header");
        fclose(input_file);
        return -1;
    }

    long total_size = get_file_size(input_file) - ftell(input_file);

    // Derive key
    unsigned char key[KEY_SIZE];
    if (crypto_pwhash(key, KEY_SIZE,
                      password, strlen(password),
                      fcrt_header.salt,
                      crypto_pwhash_OPSLIMIT_INTERACTIVE,
                      crypto_pwhash_MEMLIMIT_INTERACTIVE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        log_error("Out of memory: key derivation failed");
        fclose(input_file);
        return -1;
    }

    crypto_secretstream_xchacha20poly1305_state state;
    if (crypto_secretstream_xchacha20poly1305_init_pull(&state, fcrt_header.stream_header, key) != 0) {
        log_error("Decrypt: stream init failed (wrong password?)");
        fclose(input_file);
        sodium_memzero(key, KEY_SIZE);
        return -1;
    }

    char *output_filename = make_dec_filename(input_filename);
    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        log_error("Decrypt: failed to open output file '%s'", output_filename);
        fclose(input_file);
        sodium_memzero(key, KEY_SIZE);
        free(output_filename);
        return -1;
    }

    log_info("Decrypting file '%s' -> '%s'", input_filename, output_filename);

    unsigned char in[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
    unsigned char out[CHUNK_SIZE];
    size_t rlen;
    long processed = 0;
    while ((rlen = fread(in, 1, sizeof(in), input_file)) > 0) {
        unsigned long long out_len;
        unsigned char tag;

        if (crypto_secretstream_xchacha20poly1305_pull(&state, out, &out_len, &tag, in, rlen, NULL, 0) != 0) {
            log_error("Decrypt: file corrupted or wrong password");
            remove_file(output_filename);
            free(output_filename);

            fclose(input_file);
            fclose(output_file);
            sodium_memzero(key, KEY_SIZE);
            return -1;
        }

        fwrite(out, 1, out_len, output_file);

        processed += rlen;
        log_progress(processed, total_size);

        if (tag & crypto_secretstream_xchacha20poly1305_TAG_FINAL) break;
    }

    log_info("File decryption complete: '%s'", output_filename);

    free(output_filename);

    fclose(input_file);
    fclose(output_file);
    sodium_memzero(key, KEY_SIZE);

    return 0;
}
