#include "db.h"

int db_link_users_to_chat(int chat_id, int user_id) {
    sqlite3 *db = db_open("test.db");
    sqlite3_stmt *stmt;
    int result = 0;
    const char *sql = "INSERT INTO chat_users (chat_id, user_id) VALUES (?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_bind_int(stmt, 1, chat_id) != SQLITE_OK) {
            logging_format(LOG_ERR, "Error bind int 1 in link_users_to_chat\n");
        }
        if (sqlite3_bind_int(stmt, 2, user_id) != SQLITE_OK) {
            logging_format(LOG_ERR, "Error bind int 2 in link_users_to_chat\n");

        }
        result = (sqlite3_step(stmt) == SQLITE_DONE);
        sqlite3_finalize(stmt);
    }
    db_close(db);
    return result;
}
