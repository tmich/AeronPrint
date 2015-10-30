#pragma once
#include "order.h"
#include "sqlite_db.h"
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
	void AddAll(std::vector<Order> & orders);

	Order Get(int id);
	std::vector<Order> GetAll();
	
	void Remove(int id);

	void Update(Order & order);
	
	int GetMaxRemoteId();
private:
	bool exists(Order order);
	void addWithinTransaction(sqlite::Transaction & trn, Order & order);
	Order doMakeOrder(sqlite::Cursor & crs);
	void loadItemsForOrder(Order & order);
	/*void addItemWithinTransaction(sqlite::Transaction & trn, OrderItem & item, int orderId);*/
};

class OrderItemRepository
{
public:
	OrderItemRepository();
	~OrderItemRepository();

	void Add(sqlite::Transaction & trn, OrderItem & orderItem, int orderId);
	void AddAll(sqlite::Transaction & trn, std::vector<OrderItem> & orderItems, int orderId);

	std::vector<OrderItem> GetAll(int orderId);
private:
	OrderItem doMakeOrderItem(sqlite::Cursor & crs);
};

