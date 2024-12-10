#include "db.h"

void db_close(sqlite3 *db) {
    sqlite3_close(db);
}

