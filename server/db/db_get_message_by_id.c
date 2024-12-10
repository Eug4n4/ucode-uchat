#include "db.h"

t_message *db_get_message_by_id(int message_id) {
    sqlite3 *db = db_open("test.db");
    sqlite3_stmt *stmt;
    t_message *message = NULL;
    const char *sql = "SELECT id, receiver_id, message, timestamp FROM messages WHERE id = ?1;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, message_id); 
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            logging_format(LOG_ERR, "Error getting message with id %d: %s", message_id, sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            db_close(db);
            return message;
        }
        message = malloc(sizeof(t_message));
        message->id = sqlite3_column_int(stmt, 0);
        message->receiver_id = sqlite3_column_int(stmt, 1);
        message->content = mx_strdup((char *)sqlite3_column_text(stmt, 2));
        message->timestamp = sqlite3_column_int64(stmt, 3);
        message->sender_username = NULL;
        
    } else {
        logging_format(LOG_ERR, "Error getting message with id %d: %s", message_id, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    db_close(db);
    return message;
}

