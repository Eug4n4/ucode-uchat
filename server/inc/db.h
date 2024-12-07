#ifndef DB_H
#define DB_H

#include "server.h"

sqlite3       *db_open(const char *dbname);
void           db_close(sqlite3 *db);
t_user        *db_get_user_by_username(const char *username);
t_user        *db_get_user_by_id(int id);
t_users       *db_get_all_users_exclude_id(int user_id);
int            db_add_new_user(const char *username, const char *display_name, const char *password, int *new_user_id);
int            db_check_existing_chat(int user1_id, int user2_id);
int            db_create_chat(const char *chat_name, int chat_type);
int            db_link_users_to_chat(int chat_id, int user_id);
int            db_save_message(int sender_id, int chat_id, const char *message, int64_t *timestamp);
t_client_node *db_get_online_chat_users(t_server_state *state, int chat_id);
bool           db_check_user_in_chat(int user_id, int chat_id);
t_chats       *db_get_all_user_chats(int user_id);
t_messages    *db_get_messages_for_chat(int chat_id);
int            db_get_chat_members_count(int chat_id);
t_chat        *db_get_last_created_chat(void);
t_message *db_get_last_created_message(void);
int            db_change_display_name(int user_id, const char *new_display_name);
#endif  // DB_H
