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

	auto items = jsonValue[L"items"].as_array();
	
	for (auto it = items.cbegin(); it != items.cend(); it++)
	{
		OrderItem item;
		auto j = *it;
		item.SetRemoteId(j[L"id"].as_integer());
		item.SetName(j[L"name"].as_string());
		item.SetCode(j[L"code"].as_string());
		item.SetNotes(j[L"notes"].as_string());
		item.SetQty(j[L"qty"].as_integer());
		order.Items.push_back(item);
	}

	return order;
}
