#include "AeronClient.h"



AeronClient::AeronClient()
{
}


AeronClient::~AeronClient()
{
}

Order* AeronClient::GetOrder(int id)
{
	//pplx::task<void> requestTask = concurrency::streams::stringstream::open_ostream()
	// Create http_client to send the request.
	//http_client client(U("http://93.145.253.33/api/v1.0/orders/"));

	auto bstr = std::make_shared<concurrency::streams::basic_ostream<char>>();
	http_client client(U("http://192.168.56.1:5000/api/v1.0/orders"));
	
	// Open stream to output file.
	pplx::task<http_response> resp = client.request(methods::GET, U("/from/") + id);
	
	resp.then([=](pplx::task<http_response> task)
	{
		http_response response = task.get();
		
		//printf("Received response status code:%u\n", response.status_code());
		return response.body().read_to_end(bstr->streambuf());
	})
		// Close the file stream.
		.then([=](size_t)
	{
		return bstr->close();
	});

	return new Order();
}

vector<Order*> AeronClient::GetOrders(int lastId)
{
	return vector<Order*>();
}

