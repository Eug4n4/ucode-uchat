#include "db.h"

int db_check_existing_chat(int user1_id, int user2_id) {
    sqlite3 *db = db_open("test.db");
    sqlite3_stmt *stmt;
    int chat_id = -1;
    const char *sql = "SELECT c.id "
                      "FROM chat_users cu1 "
                      "JOIN chat_users cu2 ON cu1.chat_id = cu2.chat_id "
                      "JOIN chats c ON cu1.chat_id = c.id "
                      "WHERE cu1.user_id = ? AND cu2.user_id = ? "
                      "AND c.type = 0";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_bind_int(stmt, 1, user1_id) != SQLITE_OK) {
            logging_format(LOG_ERR, "Cannot bind int 1 in existing chat\n");
        }
        if (sqlite3_bind_int(stmt, 2, user2_id) != SQLITE_OK) {
            logging_format(LOG_ERR, "Cannot bind int 2 in existing chat\n");

        }
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            chat_id = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    db_close(db);
    return chat_id;
}
