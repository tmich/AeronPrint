#include "order_paginator.h"
#include <algorithm>

OrderPaginator::OrderPaginator(std::vector<Order> data, size_t pageSize)
	:Paginator(data, pageSize)
{

}

OrderPaginator::~OrderPaginator()
{
}

void OrderPaginator::Update(Order order)
{
	auto found = std::find_if(data_.begin(), data_.end(), [&](const Order& _ord) {return order.GetId() == _ord.GetId(); });
	if (found != data_.end())
	{
		*found = order;
		//data_.erase(found);
		//data_.insert(found, order);
	}
}
