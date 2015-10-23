#include "order_generator.h"

OrderGenerator::OrderGenerator()
{
}


OrderGenerator::~OrderGenerator()
{
}

Order OrderGenerator::GetOrderFromJson(json::value jsonValue)
{
	Order order;
	order.SetRemoteId(jsonValue[L"id"].as_integer());
	order.SetCustomerCode(jsonValue[L"customer_code"].as_string());
	std::wstring customerName = jsonValue[L"customer_name"].as_string();
	order.SetCustomerName(jsonValue[L"customer_name"].as_string());
	order.SetCreationDate(jsonValue[L"creation_date"].as_string());

	return order;
}
