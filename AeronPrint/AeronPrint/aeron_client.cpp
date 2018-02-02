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
		std::wstringstream ws;
		ws << cfg.ApiPath() << L"last_orders/" << lastId;

		// Create http_client to send the request.
		http_client_config config;
		config.set_timeout(utility::seconds(60));
		http_client client(ws.str(), config);

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

