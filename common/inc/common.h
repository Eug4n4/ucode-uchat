#ifndef COMMON_H
#define COMMON_H
#include "includes.h"
#include "defines.h"
#include "enums.h"


SSL_CTX *setup_ssl_context(bool is_server); 
bool check_username(const char *username);
bool check_password(const char *password);
char *mx_strdup(const char *s);
#endif //COMMON_H
