#ifndef DB_H
#define DB_H

#include "server.h"

sqlite3 *db_open(const char *dbname);
void db_close(sqlite3 *db);
t_user *db_get_user_by_login(const char *login);

#endif // DB_H

