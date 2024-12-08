#include "db.h"

t_message *db_delete_message_by_id(int message_id) {
    sqlite3 *db = db_open("test.db");
    sqlite3_stmt *stmt;
    t_message *to_delete = NULL;
    const char *sql = "DELETE FROM messages WHERE id = ?1;";
    to_delete = db_get_message_by_id(message_id);
    
    if (!to_delete) {
        return to_delete;
    }
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, message_id);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            logging_format(LOG_ERR, "Error deleting message %s\n", sqlite3_errmsg(db));
            
        }
    }

    sqlite3_finalize(stmt);
    db_close(db);
    return to_delete;
}


