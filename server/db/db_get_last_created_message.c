#include "db.h"

t_message *db_get_last_created_message(void) {
    sqlite3 *db = db_open("test.db");
    sqlite3_stmt *stmt;
    t_message *message = NULL;
    const char *sql = "SELECT * FROM 'messages' ORDER BY id DESC LIMIT 1";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            char *content = mx_strdup((char *)sqlite3_column_text(stmt, 2));
            int timestamp = sqlite3_column_int(stmt, 3);

            message = malloc(sizeof(t_message));
            message->id = id;
            message->content = content;
            message->timestamp = timestamp;
        }
    }

    sqlite3_finalize(stmt);
    db_close(db);
    return message;
}

