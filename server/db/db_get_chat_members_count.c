#include "db.h"

int db_get_chat_members_count(int chat_id) {
    sqlite3 *db = db_open("test.db");
    sqlite3_stmt *stmt;
    int result = -1;
    const char *sql = "SELECT COUNT(*) FROM chat_users WHERE chat_id = ?1;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        logging_format(LOG_ERR, "Error in db_get_chat_members_count: %s\n", sqlite3_errmsg(db));
        return result;
    }
    sqlite3_bind_int(stmt, 1, chat_id);
    int step_res = sqlite3_step(stmt);
    
    if (step_res == SQLITE_ROW) {
        result = sqlite3_column_int(stmt, 0);
    } else {
        logging_format(LOG_ERR, "step_res in db_get_chat_members_count != SQLITE_ROW: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    db_close(db);
    return result;
}


