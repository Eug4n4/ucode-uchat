#include "db.h"

void create_chat_users_table(sqlite3* db) {
    int ok = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS chat_users("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                              "chat_id INTEGER NOT NULL,"
                              "user_id INTEGER NOT NULL,"
                              "FOREIGN KEY(chat_id) REFERENCES chats(id),"
                              "FOREIGN KEY(user_id) REFERENCES users(id));",
                              NULL, NULL, NULL);
    if (ok != SQLITE_OK) {
        syslog(LOG_ERR, "Can't create table: %s", sqlite3_errmsg(db));

    }
}


void create_chats_table(sqlite3 *db) {
    int ok = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS chats("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                              "name TEXT NOT NULL,"
                              "type INTEGER NOT NULL);",
                              NULL, NULL, NULL);
    if (ok != SQLITE_OK) {
        syslog(LOG_ERR, "Can't create table: %s", sqlite3_errmsg(db));

        // printf("Can't create users table: %s\n", sqlite3_errmsg(db));
    }
}


void create_users_table(sqlite3 *db) {
    int ok = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS users("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                               "username TEXT UNIQUE NOT NULL,"
                               "password TEXT NOT NULL,"
                               "display_name TEXT NOT NULL);",
                                NULL, NULL, NULL);
    if (ok != SQLITE_OK) {
        syslog(LOG_ERR, "Can't create table: %s", sqlite3_errmsg(db));

        // printf("Can't create users table: %s\n", sqlite3_errmsg(db));
    }
}

sqlite3 *db_open(const char *dbname) {
    sqlite3 *db = NULL;
    int ok = sqlite3_open(dbname, &db);

    if (ok != SQLITE_OK) {
        syslog(LOG_ERR, "Can't open database: %s", sqlite3_errmsg(db));
        // printf("Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    create_users_table(db);
    create_chats_table(db);
    create_chat_users_table(db);

    return db;

}



