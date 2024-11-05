#include "db.h"

void send_response(cJSON *response, t_accepted_client *client) {
    char *response_str = cJSON_Print(response);

    write(client->client_fd, response_str, strlen(response_str));
}

