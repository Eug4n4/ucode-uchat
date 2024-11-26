#include "server.h"


char *hash_to_hex(unsigned char *hash) {
    char *hex_hash = malloc(sizeof(char) * (HASH_SIZE * 2 + 1));

    for (int i = 0; i < HASH_SIZE; ++i) {
        sprintf(hex_hash + (i * 2), "%02x", hash[i]);
    }
    hex_hash[HASH_SIZE * 2] = '\0';

    return hex_hash;
}

