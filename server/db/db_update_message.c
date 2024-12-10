#include "db.h"

t_message *db_update_message(int message_id, const char *content) {
    sqlite3 *db = db_open("test.db");
    sqlite3_stmt *stmt;
    t_message *message = NULL;
    const char *sql = "UPDATE messages SET message = ?1 WHERE id = ?2;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, content, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, message_id);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            logging_format(LOG_ERR, "Error updating message %s", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            db_close(db);
            return message;
        }
        sqlite3_finalize(stmt);
        db_close(db);
        message = db_get_message_by_id(message_id);
    }
    
    return message;
}

