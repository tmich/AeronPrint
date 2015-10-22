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

	//void SetField(std::wstring name, json::value value);
	Order GetOrderFromJson(json::value jsonValue);
//private:
	//std::map<std::wstring, FieldValue> fieldMap;
	//Order order;
};

