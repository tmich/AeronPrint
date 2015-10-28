#pragma once
#include "order.h"
#include "sqlite_db.h"
//#include "sqlite3.h"
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
	void Add(SQLiteDB & db, Order & order);
	void AddAll(std::vector<Order> & orders);

	Order Get(int id);
	std::vector<Order> GetAll();
	
	void Remove(int id);

	void Update(Order order);
	
	int GetMaxRemoteId();
private:
	bool exists(Order order);
	Order build(IResult * res);

	void addItem(SQLiteDB & db, OrderItem & item, int orderId);

	//SQLiteDB sqlite;
	//sqlite3 * sqlite = nullptr;
};

