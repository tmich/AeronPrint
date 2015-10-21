#pragma once
#include <cpprest/http_client.h>	// HTTP Client
#include <cpprest/json.h>			// JSON library
#include <cpprest/uri.h>            // URI library
#include <string>
#include <vector>
#include "Order.h"

using namespace utility;				// Common utilities like string conversions
using namespace web;					// Common features like URIs.
using namespace web::http;				// Common HTTP functionality
using namespace web::http::client;		// HTTP client features
using namespace concurrency::streams;   // Asynchronous streams
using namespace std;

class AeronClient
{
public:
	AeronClient();
	~AeronClient();
	Order* GetOrder(int id);
	vector<Order*> GetOrders(int lastId);
};

