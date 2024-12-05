#include "db.h"

t_chat *db_get_last_created_chat(void) {
    sqlite3 *db = db_open("test.db");
    sqlite3_stmt *stmt;
    t_chat *chat = NULL;
    const char *sql = "SELECT * FROM chats ORDER BY id DESC LIMIT 1";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int chat_id = sqlite3_column_int(stmt, 0);
            char *chat_name = mx_strdup((char *)sqlite3_column_text(stmt, 1));
            int chat_type = sqlite3_column_int(stmt, 2);

            chat = create_chat(chat_id, chat_name, chat_type);
        }
    }

    sqlite3_finalize(stmt);
    db_close(db);
    return chat;
}

