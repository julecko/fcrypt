#ifndef FILE_FORMAT_H
#define FILE_FORMAT_H

#include <stdio.h>
#include <stdint.h>
#include <sodium.h>

#define FCRT_MAGIC "FCRT"
#define FCRT_MAGIC_LEN 4

#define FCRT_VERSION_1 1

typedef struct {
    uint8_t version;
    unsigned char salt[crypto_pwhash_SALTBYTES];
    unsigned char stream_header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
} fcrt_header_t;

int fcrt_write_header(FILE *f, const fcrt_header_t *hdr);
int fcrt_read_header(FILE *f, fcrt_header_t *hdr);


#endif