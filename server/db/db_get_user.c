#include "db.h"

t_user *db_get_user_by_username(const char *username) {
    sqlite3 *db = db_open("test.db");
    sqlite3_stmt *stmt = NULL;
    t_user *user = NULL;
    char *sql = "SELECT "
                "id, "
                "username, "
                "password, "
                "display_name "
                "FROM users WHERE username = ?1;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, username, -1, NULL);

    int res = sqlite3_step(stmt);

    if (res == SQLITE_ROW) {
        user = create_user();
        user->id = sqlite3_column_int(stmt, 0);
        user->username = strdup((char *)sqlite3_column_text(stmt, 1));
        user->password = malloc(sizeof(unsigned char) * (HASH_SIZE * 2 + 1));
        memcpy(user->password, sqlite3_column_text(stmt, 2), HASH_SIZE * 2 + 1);
        user->display_name = strdup((char *)sqlite3_column_text(stmt, 3));
    }

    sqlite3_finalize(stmt);
    db_close(db);
    return user;
}
