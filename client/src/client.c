#define _POSIX_C_SOURCE 1
#include "client.h"
#include "../gui/gui_structs.h"

t_gtk_sign_in *gtk_sign_in;

char *stdin_str_read(char *buf) {
    fgets(buf, BUF_SIZE - 1, stdin);
    buf[strlen(buf) - 1] = '\0';
    return buf;
}

SSL_CTX *setup_ssl_client_context(void) {
    const SSL_METHOD *method = TLS_client_method();
    SSL_CTX *ctx = NULL;

    if (method) {
        ctx = SSL_CTX_new(method);
    }
    if (!ctx) {
        printf("Error in setup_ssl_client_context\n");
        return NULL;
    }
    return ctx;
}

void on_login_clicked(GtkButton *button, gpointer data) {
    if (!data) return;

    t_gtk_sign_in *sign_in_data = (t_gtk_sign_in *)data;
    const gchar *username = gtk_entry_get_text(sign_in_data->entry_username);
    const gchar *password = gtk_entry_get_text(sign_in_data->entry_password);

    if (strlen(username) == 0 || strlen(password) == 0) {
        gtk_label_set_text(sign_in_data->label_error, "Fields cannot be empty");
        return;
    }

    gtk_label_set_text(sign_in_data->label_error, "");

    // Create a login request using cJSON
    cJSON *request = create_request_login();
    cJSON *content = cJSON_GetObjectItemCaseSensitive(request, KEY_CONTENT);
    cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_USERNAME, cJSON_CreateString(username));
    cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_PASSWORD, cJSON_CreateString(password));

    char *str = cJSON_PrintUnformatted(request);
    if (SSL_write(sign_in_data->ssl, str, strlen(str)) < 0) {
        printf("Error writing to socket\n");
        gtk_label_set_text(sign_in_data->label_error, "Error communicating with server");
        free(str);
        cJSON_Delete(request);
        return;
    }
    free(str);
    cJSON_Delete(request);

    char buf[BUF_SIZE] = {0};
    if (SSL_read(sign_in_data->ssl, buf, BUF_SIZE) <= 0) {
        printf("Error reading response\n");
        gtk_label_set_text(sign_in_data->label_error, "Error receiving server response");
        return;
    }

    // Parse and handle server response
    cJSON *server_response = cJSON_Parse(buf);
    if (!server_response) {
        gtk_label_set_text(sign_in_data->label_error, "Invalid server response");
        return;
    }

    cJSON *response_type = cJSON_GetObjectItemCaseSensitive(server_response, KEY_RESPONSE_TYPE);
    cJSON *response_content = cJSON_GetObjectItemCaseSensitive(server_response, KEY_CONTENT);
    cJSON *response_message = cJSON_GetObjectItemCaseSensitive(response_content, KEY_MESSAGE);

    if (response_message) {
        gtk_label_set_text(sign_in_data->label_error, response_message->valuestring);
    }

    if (response_type && (response_type->valueint == 1 || response_type->valueint == 3)) {
        // Successful login, proceed as needed (e.g., open a new window or transition)
        exit(0);
    }

    cJSON_Delete(server_response);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <host> <port>\n", argv[0]);
        exit(1);
    }

    struct sockaddr_in serv_address;
    struct hostent *server;
    int port = atoi(argv[2]);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        printf("Failed to create socket\n");
        exit(1);
    }

    server = gethostbyname(argv[1]);
    if (!server) {
        printf("Host not found\n");
        exit(1);
    }

    memset(&serv_address, 0, sizeof(serv_address));
    serv_address.sin_family = AF_INET;
    memcpy(&serv_address.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_address.sin_port = htons(port);

    if (connect(server_fd, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0) {
        printf("Connection failed\n");
        exit(1);
    }

    SSL_CTX *ctx = setup_ssl_client_context();
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, server_fd);
    if (SSL_connect(ssl) <= 0) {
        printf("SSL connection failed\n");
        exit(1);
    }

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Initialize GUI and pass SSL connection
    t_gtk_sign_in *gtk_sign_in = create_gtk_sign_in_data();
    if (!gtk_sign_in) {
        printf("Failed to create GTK sign-in data\n");
        exit(1);
    }
    gtk_sign_in->ssl = ssl;

    init_gui(argc, argv, gtk_sign_in);

    SSL_CTX_free(ctx);
    SSL_free(ssl);
    close(server_fd);

    return 0;
}
