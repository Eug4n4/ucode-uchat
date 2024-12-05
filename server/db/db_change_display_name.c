#include "db.h"

int db_change_display_name(int user_id, const char *new_display_name) {
    sqlite3      *db   = db_open("test.db");
    sqlite3_stmt *stmt = NULL;
    const char   *sql  = "UPDATE users SET display_name = ? WHERE id = ?;";

    int ok = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (ok != SQLITE_OK) {
        logging_format(LOG_ERR, "Failed to prepare statement: %s", sqlite3_errmsg(db));
        db_close(db);
        return ok;
    }

    sqlite3_bind_text(stmt, 1, new_display_name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, user_id);

    ok = sqlite3_step(stmt);
    if (ok != SQLITE_DONE) {
        logging_format(LOG_ERR, "Failed to update display name: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        db_close(db);
        return ok;
    }

    sqlite3_finalize(stmt);
    db_close(db);
    logging_format(LOG_INFO, "Successfully updated display name for user ID %d", user_id);
    return SQLITE_OK;
}
