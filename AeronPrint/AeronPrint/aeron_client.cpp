#include "aeron_client.h"
#include "config.h"


AeronClient::AeronClient()
{
}


AeronClient::~AeronClient()
{
}

pplx::task<vector<Order>> AeronClient::GetAll(int lastId)
{
	return pplx::create_task([lastId]
	{
		Config cfg;
		cfg.SetPath(wxT("/API"));
		std::wstringstream ws;
		//ws << L"http://192.168.56.1:5000/api/v1.0/last_orders/" << lastId;
		//ws << L"http://localhost:5000/api/v1.0/last_orders/" << lastId;
		ws << cfg.Read("Path") << L"last_orders/" << lastId;
		http_client client(ws.str());

		return client.request(methods::GET);
	}).then([](http_response response)
	{
		if (response.status_code() == status_codes::OK)
		{
			return response.extract_json();
		}

		return pplx::create_task([] { return json::value(); });

	}).then([=](json::value jsonValue)
	{
		OrderGenerator generator;
		vector<Order> orders;

		auto json_list = jsonValue.at(L"json_list").as_array();
		for (auto iterArray = json_list.cbegin(); iterArray != json_list.cend(); ++iterArray)
		{
			const json::value &arrayValue = *iterArray;

			auto order = generator.GetOrderFromJson(arrayValue);
			orders.push_back(order);
		}

		return orders;
	});
}

