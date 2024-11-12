#include "db.h"

int db_save_message(int sender_id, int chat_id, const char *message) {
    sqlite3 *db = db_open("test.db");
    if (db == NULL) {
        return -1;
    }

    if (!db_check_user_in_chat(sender_id, chat_id)) {
        sqlite3_close(db);
        syslog(LOG_ERR, "User %d is not part of chat %d", sender_id, chat_id);
        return -1;
    }

    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO messages (sender_id, receiver_id, message, timestamp) "
                      "VALUES (?, ?, ?, strftime('%s', 'now'));";
    int success = -1;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, sender_id);
        sqlite3_bind_int(stmt, 2, chat_id);
        sqlite3_bind_text(stmt, 3, message, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            success = 0;
        }
        sqlite3_finalize(stmt);
    } else {
        syslog(LOG_ERR, "Error inserting message: %s", sqlite3_errmsg(db));
    }

    sqlite3_close(db);
    return success;
}
