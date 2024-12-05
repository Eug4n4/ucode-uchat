#include "db.h"

t_messages *db_get_messages_for_chat(int chat_id) {
    sqlite3      *db = db_open("test.db");
    sqlite3_stmt *stmt;
    t_messages   *messages = create_message_list();
    char         *query    = "SELECT id, sender_id, message, timestamp FROM messages WHERE receiver_id = ? ORDER BY timestamp ASC";

    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        logging_format(LOG_ERR, "Failed to prepare statement: %s", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_int(stmt, 1, chat_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        t_user *user = db_get_user_by_id(sqlite3_column_int(stmt, 1));
        t_message *msg       = malloc(sizeof(t_message));
        msg->next = NULL;
        msg->id              = sqlite3_column_int(stmt, 0);
        msg->sender_username = mx_strdup(user->username);
        msg->content         = mx_strdup((const char *)sqlite3_column_text(stmt, 2));
        msg->timestamp       = sqlite3_column_int64(stmt, 3);

        append_message_to_list(messages, msg);
        free_user(user);
    }

    sqlite3_finalize(stmt);
    db_close(db);
    return messages;
}
