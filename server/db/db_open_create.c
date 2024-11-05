#include "db.h"


void create_users_table(sqlite3 *db) {
    int ok = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS users("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                               "login TEXT UNIQUE NOT NULL,"
                               "password TEXT NOT NULL,"
                               "display_name TEXT NOT NULL,"
                               "first_name TEXT,"
                               "last_name TEXT);",
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

    return db;

}



