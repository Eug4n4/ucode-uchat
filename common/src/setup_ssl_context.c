#include "common.h"

SSL_CTX *setup_ssl_context(bool is_server) {
    const SSL_METHOD *method = NULL;

    if (is_server) {
        method = TLS_server_method();
    } else {
        method = TLS_client_method();
    }
    
    SSL_CTX *ctx = NULL;

    if (method) {
        ctx = SSL_CTX_new(method);
    }
    if (method == NULL || ctx == NULL) {
        return NULL;
    }
   
    return ctx;
}

