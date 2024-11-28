#include "db.h"

// Get all users from the database. User with user_id won't be in the resulting list
t_users *db_get_all_users_exclude_id(int user_id) {
    sqlite3 *db = db_open("test.db");
    sqlite3_stmt *stmt;
    char *query = "SELECT * FROM users WHERE id != ?1;";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    sqlite3_bind_int(stmt, 1, user_id);
    
    int step_res = sqlite3_step(stmt);
    t_users *users = create_users(NULL);

    while (step_res == SQLITE_ROW) {
        int user_id = sqlite3_column_int(stmt, 0);
        char *username = (char *)sqlite3_column_text(stmt, 1);
        char *password = (char *)sqlite3_column_text(stmt, 2);
        char *display_name = (char *)sqlite3_column_text(stmt, 3);
        t_user *user = create_user_with_params(user_id, username, password, display_name);

        add_users_front(&users, user);
        step_res = sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
    db_close(db);


    return users;
}

