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
	std::vector<Order> CheckNewOrders();

	void SaveOrders(std::vector<Order> & orders);
	void Save(Order & order);
	Order GetOrderById(int id);
private:
	int getLastOrderRemoteId();
};

