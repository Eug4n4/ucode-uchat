#include "server.h" 


unsigned char *hash_password(const char *password, int password_len) {
    unsigned char *hash = malloc(sizeof(unsigned char) * HASH_SIZE);

    if (PKCS5_PBKDF2_HMAC(password, password_len, NULL, 0, 10000, EVP_sha512(), HASH_SIZE, hash) != 1) {
        printf("Error generating hash\n");
        return NULL;
    }
    return hash;
}

