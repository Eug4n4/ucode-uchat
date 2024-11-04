#include "db.h"


t_user *db_get_user_by_login(const char *login) {
    sqlite3 *db = db_open("test.db");
    sqlite3_stmt *stmt;
    t_user *user = NULL;
    char *sql = "SELECT "
                "id, "
                "login, "
                "password, "
                "display_name, "
                "first_name, "
                "last_name "
                "FROM users WHERE login = ?1;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, login, -1, NULL);

    int res = sqlite3_step(stmt);

    if (res == SQLITE_ROW) {
        user = create_user();
        user->id = sqlite3_column_int(stmt, 0);
        user->login = strdup((char *)sqlite3_column_text(stmt, 1));
        user->password = strdup((char *)sqlite3_column_text(stmt, 2));
        user->display_name = strdup((char *)sqlite3_column_text(stmt, 3));
        user->first_name = strdup((char *)sqlite3_column_text(stmt, 4));
        user->last_name = strdup((char *)sqlite3_column_text(stmt, 5));
    }
    sqlite3_finalize(stmt);
    db_close(db);
    return user;
}

