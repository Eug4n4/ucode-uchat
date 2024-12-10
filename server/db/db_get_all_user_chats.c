#include "db.h"

t_chats *db_get_all_user_chats(int user_id) {
    sqlite3      *db = db_open("test.db");
    sqlite3_stmt *stmt;

    char *query_chats = "SELECT chat_id, name, type "
                        "FROM chats JOIN chat_users "
                        "ON chats.id = chat_id WHERE user_id = ?1;";
    if (sqlite3_prepare_v2(db, query_chats, -1, &stmt, NULL) != SQLITE_OK) {
        logging_format(LOG_ERR, "Error: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_int(stmt, 1, user_id);
    int      step_res = sqlite3_step(stmt);
    t_chats *chats    = create_chats(NULL);
    while (step_res == SQLITE_ROW) {
        int   chat_id = sqlite3_column_int(stmt, 0);
        char *name    = mx_strdup((char *)sqlite3_column_text(stmt, 1));
        int   type    = sqlite3_column_int(stmt, 2);

        t_chat *chat = create_chat(chat_id, name, type);
        add_chat_front(&chats, chat);
        step_res = sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
    db_close(db);

    return chats;
}

