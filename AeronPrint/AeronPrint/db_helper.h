#pragma once
#include <sqlite3.h>

class DbHelper
{
public:
	DbHelper();
	~DbHelper();

	operator sqlite3*();
private:
	sqlite3 * db;
	void create_tables();
};

