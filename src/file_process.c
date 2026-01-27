#include "crypt/crypt.h"
#include "util.h"
#include "cli.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#endif

void process_file(const char *filename, const char *password, bool encrypting) {
    if (!file_exist(filename)) {
        fprintf(stderr, "File %s does not exist\n", filename);
        return;
    }

    if (encrypting) {
        encrypt(filename, password);
    } else {
        decrypt(filename, password);
    }

    if (take_answer("Delete old file?")) {
        remove_file(filename);
    }
}

#ifdef _WIN32

void process_all_files(const char *pattern, const char *password, bool encrypting) {
    WIN32_FIND_DATAA data;
    HANDLE h = FindFirstFileA(pattern, &data);

    if (h == INVALID_HANDLE_VALUE) {
        fputs("Getting all files in directory failed\n", stderr);
        return;
    }

    do {
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue;
        }
        process_file(data.cFileName, password, encrypting);
    } while (FindNextFileA(h, &data));

    FindClose(h);
}

#else

void process_all_files(const char *pattern, const char *password, bool encrypting) {
    DIR *dir = opendir(".");
    if (!dir) {
        perror("Getting all files in directory failed");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        // Skip directories
        struct stat st;
        if (stat(entry->d_name, &st) == 0 && S_ISDIR(st.st_mode)) continue;

        process_file(entry->d_name, password, encrypting);
    }

    closedir(dir);
}

#endif