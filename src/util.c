#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *make_enc_filename(const char *filename) {
    size_t len = strlen(filename);
    const char *enc_ext = ".enc";

    char *out = malloc(len + strlen(enc_ext) + 1);
    if (!out) return NULL;

    memcpy(out, filename, len);
    memcpy(out + len, enc_ext, strlen(enc_ext) + 1);

    return out;
}

char *make_dec_filename(const char *filename) {
    size_t len = strlen(filename);

    const char *enc_ext = ".enc";
    const char *dec_ext = ".dec";
    size_t enc_ext_len = strlen(enc_ext);
    size_t dec_ext_len = strlen(dec_ext);

    char *out;

    if (len > enc_ext_len && strcmp(filename + len - enc_ext_len, enc_ext) == 0) {
        out = malloc(len - enc_ext_len + 1);
        strncpy(out, filename, len - enc_ext_len);
        out[len - enc_ext_len] = '\0';
    } else {
        out = malloc(len + dec_ext_len + 1);
        strcpy(out, filename);
        strcat(out, dec_ext);
    }

    return out;
}