#include "db.h"

void send_response(cJSON *response, t_accepted_client *client) {
    char *response_str = cJSON_PrintUnformatted(response);
    int res = SSL_write(client->ssl, response_str, strlen(response_str));
    logging_format(LOG_INFO, "JSON Response: %s\n", response_str);
    if (res <= 0) {
        logging_format(LOG_ERR, "Error sending response\n");
    }
    free(response_str);
}

