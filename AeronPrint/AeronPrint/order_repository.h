#pragma once
#include "order.h"
#include "sqlite_db.h"
#include "sqlite3.h"
#include "constants.h"
#include <string>
#include <exception>
#include <sstream>
#include <vector>
#include <Windows.h>

class OrderRepository
{
public:
	OrderRepository();
	~OrderRepository();

	void Add(Order & order);
	void Add(sqlite3 * db, Order & order);
	void AddAll(std::vector<Order> & orders);

	Order Get(int id);
	std::vector<Order> GetAll();
	
	void Remove(int id);

	void Update(Order order);
	
	int GetMaxRemoteId();
private:
	bool exists(Order order);
	Order build(sqlite3_stmt * statement);
	std::string to_string(std::wstring s);
	std::string to_string(const unsigned char * s);
	std::wstring s2ws(const std::string& str);

	//SQLiteDB sqlite;
	sqlite3 * sqlite;
};

