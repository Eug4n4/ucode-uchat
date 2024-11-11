#include "db.h"

int db_add_new_user(const char *username, const char *display_name, const char *password) {
    sqlite3 *db = db_open("test.db");
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO users (username, display_name, password) VALUES (?, ?, ?);";

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);


    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, display_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, password, -1, SQLITE_STATIC);

    sqlite3_step(stmt);


    sqlite3_finalize(stmt);
    db_close(db);
    return 0;
}
