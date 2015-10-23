#pragma once
#include <cpprest/http_client.h>	// HTTP Client
#include <cpprest/json.h>			// JSON library
#include <cpprest/uri.h>            // URI library
#include <string>
#include <vector>
#include "order.h"
#include "order_generator.h"

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
	pplx::task<vector<Order>> GetAll(int lastId);
};
