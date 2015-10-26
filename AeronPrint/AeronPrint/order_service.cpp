#include "order_service.h"



OrderService::OrderService()
{
}


OrderService::~OrderService()
{
}

//std::vector<Order> OrderService::GetNewOrders()
//{
//	/*vector<Order> orders;
//	AeronClient client;
//
//	OrderRepository repo;
//	int maxId = repo.GetMaxRemoteId();
//	
//	auto task = client.GetAll(maxId);
//	try
//	{
//		orders = task.get();
//	}
//	catch (const http_exception&)
//	{
//		throw ConnectionException();
//	}
//	catch (const std::exception&)
//	{
//		throw;
//	}*/
//
//	//try
//	//{
//	//	OrderRepository repo;
//	//	
//	//	// salvo nel db
//	//	for (int i = 0; i < orders.size(); i++)
//	//	{
//	//		repo.Add(orders.at(i));
//	//	}
//
//	//}
//	//catch (const std::exception& exc)
//	//{
//	//	throw DatabaseException(exc.what());
//	//}
//
//	//return orders;
//}

std::vector<Order> OrderService::GetAllOrders()
{
	//GetNewOrders();
	OrderRepository repo;
	return repo.GetAll();
}

int OrderService::getLastOrderRemoteId()
{
	OrderRepository repo;
	int maxId = repo.GetMaxRemoteId();
	return maxId;
}

void OrderService::CheckNewOrders()
{
	vector<Order> orders;
	AeronClient client;

	OrderRepository repo;
	int lastId = repo.GetMaxRemoteId();

	auto task = client.GetAll(lastId);
	try
	{
		orders = task.get();
	}
	catch (const http_exception&)
	{
		throw ConnectionException();
	}
	catch (const std::exception&)
	{
		throw;
	}

	repo.AddAll(orders);
}

void OrderService::SaveOrders(vector<Order> & orders)
{
	OrderRepository repo;
	repo.AddAll(orders);
}

void OrderService::Save(Order order)
{
	OrderRepository repo;
	if (order.GetId() == 0)
	{
		repo.Add(order);
	}
	else
	{
		repo.Update(order);
	}
}

Order OrderService::GetOrderById(int id)
{
	OrderRepository repo;
	return repo.Get(id);
}

DatabaseException::DatabaseException(const std::string& message)
	:message(message)
{
	
}
