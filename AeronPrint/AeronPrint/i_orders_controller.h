#pragma once
class IOrdersController
{
public:
	virtual void OnReadOrder(int id) = 0;

	virtual ~IOrdersController();
};

