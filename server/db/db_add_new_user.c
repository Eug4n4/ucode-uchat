#include "db.h"

int db_add_new_user(const char *username, const char *display_name, const char *password, int *new_user_id) {
    sqlite3 *db = db_open("test.db");
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO users (username, display_name, password) VALUES (?, ?, ?);";
    unsigned char *hash = hash_password(password, strlen(password));
    char *hex_hash = hash_to_hex(hash);
    
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, display_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, hex_hash, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        logging_format(LOG_ERR, "cannot step in add_new_user: %s\n", sqlite3_errmsg(db));
    }

    *new_user_id = (int)sqlite3_last_insert_rowid(db);
    free(hex_hash);
    free(hash);
    sqlite3_finalize(stmt);
    db_close(db);
    return 0;
}


