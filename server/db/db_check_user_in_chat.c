#include "db.h"

bool db_check_user_in_chat(int user_id, int chat_id) {
    sqlite3      *db = db_open("test.db");
    sqlite3_stmt *stmt;
    const char   *sql        = "SELECT 1 FROM chat_users WHERE user_id = ? AND chat_id = ? LIMIT 1;";
    bool          is_in_chat = false;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user_id);
        sqlite3_bind_int(stmt, 2, chat_id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            is_in_chat = true;
        }
        sqlite3_finalize(stmt);
    } else {
        syslog(LOG_ERR, "Error checking user in chat: %s", sqlite3_errmsg(db));
    }

    db_close(db);
    return is_in_chat;
}
