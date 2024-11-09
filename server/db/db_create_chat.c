#include "db.h"

int db_create_chat(const char *chat_name, int chat_type) {
    sqlite3 *db = db_open("test.db");
    sqlite3_stmt *stmt;
    int chat_id = -1;
    const char *sql = "INSERT INTO chats (name, type) VALUES (?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, chat_name, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, chat_type);
        sqlite3_step(stmt);
        chat_id = sqlite3_last_insert_rowid(db);
        sqlite3_finalize(stmt);
    }
    return chat_id;
}
