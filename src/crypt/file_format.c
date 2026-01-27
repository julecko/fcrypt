#include "crypt/file_format.h"
#include <stdio.h>
#include <string.h>

int fcrt_write_header(FILE *f, const fcrt_header_t *hdr) {
    if (fwrite(FCRT_MAGIC, 1, FCRT_MAGIC_LEN, f) != FCRT_MAGIC_LEN)
        return -1;

    if (fputc(hdr->version, f) == EOF)
        return -1;

    if (fwrite(hdr->salt, 1, sizeof(hdr->salt), f) != sizeof(hdr->salt))
        return -1;

    if (fwrite(hdr->stream_header, 1, sizeof(hdr->stream_header), f) != sizeof(hdr->stream_header))
        return -1;

    return 0;
}

int fcrt_read_header(FILE *f, fcrt_header_t *hdr) {
    char magic[FCRT_MAGIC_LEN];

    if (fread(magic, 1, FCRT_MAGIC_LEN, f) != FCRT_MAGIC_LEN)
        return -1;

    if (memcmp(magic, FCRT_MAGIC, FCRT_MAGIC_LEN) != 0)
        return -1;

    int v = fgetc(f);
    if (v == EOF)
        return -1;

    hdr->version = (uint8_t)v;

    if (hdr->version != FCRT_VERSION_1)
        return -1;

    if (fread(hdr->salt, 1, sizeof(hdr->salt), f) != sizeof(hdr->salt))
        return -1;

    if (fread(hdr->stream_header, 1, sizeof(hdr->stream_header), f)
        != sizeof(hdr->stream_header))
        return -1;

    return 0;
}
