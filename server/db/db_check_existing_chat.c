#include "db.h"

int db_check_existing_chat(int user1_id, int user2_id) {
    sqlite3 *db = db_open("test.db");
    sqlite3_stmt *stmt;
    int chat_id = -1;
    const char *sql = "SELECT chat_id FROM chat_users WHERE user_id IN (?, ?) GROUP BY chat_id HAVING COUNT(DISTINCT user_id) = 2;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user1_id);
        sqlite3_bind_int(stmt, 2, user2_id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            chat_id = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    return chat_id;
}
