#pragma once
#include "orders_view.h"
#include "paginator.h"
#include "order_repository.h"
#include <agents.h>

class OrdersController
{
public:
	OrdersController(OrdersView * ordersView);
	~OrdersController();
protected:
	concurrency::timer<int> * polling_timer;

	OrdersView * view_;
	Paginator<Order> * orderPaginator;
	OrderRepository * repo;

	void checkOrders();
	
	template <class T>
	void create_timer_and_start(const T& callback, unsigned int timeInMs);
};

template<class T>
inline void OrdersController::create_timer_and_start(const T & callback, unsigned int timeInMs)
{
	concurrency::task_completion_event<void> tce;
	auto call = new concurrency::call<int>(
		[callback, tce](int)
	{
		callback();
		//tce.set();
	});

	polling_timer = new concurrency::timer<int>(timeInMs, 0, call, true);
	/*concurrency::task<void> event_set(tce);
	event_set.then([call]()
	{
		delete call;
	});*/
	polling_timer->start();
}
