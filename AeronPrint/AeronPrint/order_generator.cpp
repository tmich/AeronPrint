#include "order_generator.h"



OrderGenerator::OrderGenerator()
{
	/*fieldMap[L"id"] = FieldValue::Id;
	fieldMap[L"customer_code"] = FieldValue::CustomerCode;
	fieldMap[L"customer_name"] = FieldValue::CustomerName;
	fieldMap[L"notes"] = FieldValue::Notes;*/
}


OrderGenerator::~OrderGenerator()
{
}

/*void OrderGenerator::SetField(std::wstring name, json::value value)
{
	switch (fieldMap[name])
	{
	case FieldValue::Id:
		order.SetId(value.as_integer());
		break;

	case FieldValue::CustomerCode:
		order.SetCustomerCode(value.as_string());
		break;

	case FieldValue::CustomerName:
		order.SetCustomerName(value.as_string());
		break;

	case FieldValue::Notes:
		order.SetNotes(value.as_string());
		break;
	}
}*/

Order OrderGenerator::GetOrderFromJson(json::value jsonValue)
{
	Order order;
	order.SetId(jsonValue[L"id"].as_integer());
	order.SetCustomerCode(jsonValue[L"customer_code"].as_string());
	order.SetCustomerName(jsonValue[L"customer_name"].as_string());
	order.SetCreationDate(jsonValue[L"creation_date"].as_string());

	return order;
}
