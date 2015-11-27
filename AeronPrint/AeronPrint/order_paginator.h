#pragma once
#include "paginator.h"
#include "Order.h"

class OrderPaginator :
	public Paginator<Order>
{
public:
	OrderPaginator(std::vector<Order> data, size_t pageSize);
	virtual ~OrderPaginator();
	void Update(Order order);
};

