#pragma once
#include "aeron_client.h"
#include "order.h"
#include "order_repository.h"
#include <vector>

class OrderService
{
public:
	OrderService();
	~OrderService();

	//std::vector<Order> GetNewOrders();
	std::vector<Order> GetAllOrders();
	void CheckNewOrders();

	void SaveOrders(vector<Order> & orders);
	void Save(Order order);
	Order GetOrderById(int id);
private:
	int getLastOrderRemoteId();
};

class ConnectionException : exception {};

class DatabaseException   : exception
{
	std::string message;
public:
	DatabaseException(const std::string& message);
	virtual const char * what() const throw() { return message.c_str(); }
};

