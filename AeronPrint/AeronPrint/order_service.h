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

	std::vector<Order> GetLastOrders();
};

