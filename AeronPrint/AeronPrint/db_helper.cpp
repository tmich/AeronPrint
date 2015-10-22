#include "db_helper.h"



DbHelper::DbHelper()
{
	sqlite3_open_v2("file:data.db", &db, SQLITE_OPEN_URI | SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
}


DbHelper::~DbHelper()
{
	sqlite3_close_v2(db);
}

DbHelper::operator sqlite3*()
{
	return db;
}

void DbHelper::create_tables()
{
	char *sql = "create table if not exists orders(id integer unique, customer_code text, customer_name text, creation_date text);";
	sqlite3_exec(db, sql, NULL, NULL, NULL);
}

