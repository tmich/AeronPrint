#include "stdafx.h"
#include <CppUnitTest.h>
#include "..\AeronPrint\AeronClient.h"
#include "..\AeronPrint\Order.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AeronPrintTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			AeronClient cl;
			auto order = cl.GetOrder(1);
			Assert::IsNotNull(order);
		}

	};
}