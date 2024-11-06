#ifndef DB_H
#define DB_H

#include "server.h"

sqlite3 *db_open(const char *dbname);
void db_close(sqlite3 *db);
t_user *db_get_user_by_login(const char *login);
int db_add_new_user(const char *email, const char *display_name, const char *password);

#endif // DB_H

