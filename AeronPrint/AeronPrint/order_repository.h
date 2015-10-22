#pragma once
#include "order.h"
#include "sqlite_db.h"
#include <string>
#include <sstream>

class OrderRepository
{
public:
	OrderRepository();
	~OrderRepository();

	void Add(Order order);
	Order Get(int id);
	void Remove(int id);

	int GetMaxRemoteId();
private:
	std::string to_string(std::wstring s);
	std::wstring s2ws(const std::string& str);
};

