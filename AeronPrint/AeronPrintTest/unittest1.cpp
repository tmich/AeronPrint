#include "stdafx.h"
#include <vector>
#include <CppUnitTest.h>
#include "..\AeronPrint\aeron_client.h"
#include "..\AeronPrint\order.h"
#include "..\AeronPrint\order_service.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AeronPrintTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(AeronClientTest)
		{
			AeronClient client;
			auto task = client.GetAll(1);
			auto orders = task.get();
			Assert::IsTrue(orders.size() > 0);
		}

		TEST_METHOD(OrderServiceShouldReturnLastOrders)
		{
			OrderService service;
			auto orders = service.GetAllOrders();
			Assert::IsTrue(orders.size() > 0);
		}
	};
}