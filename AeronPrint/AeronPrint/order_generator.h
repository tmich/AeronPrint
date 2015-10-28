#pragma once
#include <map>
#include "order.h"
#include <cpprest\json.h>

using namespace web;

class OrderGenerator
{
public:
	OrderGenerator();
	~OrderGenerator();

	Order GetOrderFromJson(json::value jsonValue);
};

