#include "stdafx.h"
#include <vector>
#include <CppUnitTest.h>
#include "..\AeronPrint\aeron_client.h"
#include "..\AeronPrint\order.h"
#include "..\AeronPrint\order_repository.h"
#include "..\AeronPrint\order_service.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AeronPrintTest
{
	TEST_CLASS(OrderRepositoryTest)
	{
	public:
		
		TEST_METHOD(InsertNewOrder)
		{
			OrderRepository repo;
			Order o;
			int id = repo.GetMaxRemoteId() + 1;
			o.SetId(id);
			o.SetCustomerCode(L"TEST");
			o.SetCustomerName(L"Customer TEST");
			o.SetCreationDate(L"20151022");
			o.SetRemoteId(id);

			repo.Add(o);
		}

		TEST_METHOD(GetOrderById)
		{
			OrderRepository repo;
			auto o = repo.Get(repo.GetMaxRemoteId());
			Assert::IsFalse(o.GetRemoteId() == 0);
		}
	};
}