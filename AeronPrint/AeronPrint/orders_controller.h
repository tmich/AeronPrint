#pragma once
#include "orders_view.h"
#include "paginator.h"
#include "order_repository.h"

class OrdersController
{
public:
	OrdersController(OrdersView * ordersView);
	~OrdersController();
protected:
	OrdersView * view_;
	Paginator<Order> * orderPaginator;
	OrderRepository * repo;
};

