#pragma once
#include <map>
#include "order.h"
#include <cpprest\json.h>

using namespace web;

//enum FieldValue { Id, CustomerCode, CustomerName, Notes };

class OrderGenerator
{
public:
	OrderGenerator();
	~OrderGenerator();

	Order GetOrderFromJson(json::value jsonValue);
};

