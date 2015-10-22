#include "order_service.h"



OrderService::OrderService()
{
}


OrderService::~OrderService()
{
}

std::vector<Order> OrderService::GetLastOrders()
{
	OrderRepository repo;

	AeronClient client;
	auto task = client.GetAll(1);
	auto orders = task.get();

	// salvo nel db
	for each (Order o in orders)
	{
		repo.Add(o);
	}

	return orders;
}
