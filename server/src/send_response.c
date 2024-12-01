#include "db.h"

void send_response(cJSON *response, t_accepted_client *client) {
    char *response_str = cJSON_Print(response);
    int res = SSL_write(client->ssl, response_str, strlen(response_str));
    printf("JSON Response: %s\n", response_str);
    if (res <= 0) {
       // printf("Error sending response\n");
        logging_format(LOG_ERR, "Error sending response\n");
    }
    free(response_str);
    // write(client->client_fd, response_str, strlen(response_str));
}

