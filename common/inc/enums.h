#ifndef ENUMS_H
#define ENUMS_H

typedef enum e_request_type { REGISTRATION,
                              LOGIN,
                              CREATE_NEW_PRIVATE_CHAT,
                              GET_ALL_CHATS,
                              MESSAGE,
                              GET_CHAT_MESSAGES,
                              ALL_USERS_EXCLUDE } t_request_type;

typedef enum e_response_type { OK_LOGIN,
                               FAIL_LOGIN,
                               OK_REGISTRATION,
                               FAIL_REGISTRATION,
                               OK_CREATE_NEW_PRIVATE_CHAT,
                               FAIL_CREATE_NEW_PRIVATE_CHAT,
                               OK_GET_ALL_CHATS,
                               FAIL_GET_ALL_CHATS,
                               OK_MESSAGE,
                               FAIL_MESSAGE,
                               OK_GET_CHAT_MESSAGES,
                               FAIL_GET_CHAT_MESSAGES,
                               OK_ALL_USERS_EXCLUDE,
                               FAIL_ALL_USERS_EXCLUDE,
                               BAD_REQUEST} t_response_type;

#endif //ENUMS_H
