#pragma once
#include <string>

class Order
{
public:
	Order();
	~Order();

	int GetId() { return mId; }
	void SetId(int id) { mId = id; }

	int GetRemoteId() { return mRemoteId; }
	void SetRemoteId(int id) { mRemoteId = id; }

	std::wstring GetCustomerName() { return mCustomerName; }
	void SetCustomerName(std::wstring customerName) { mCustomerName = customerName; }

	std::wstring GetCustomerCode() { return mCustomerCode; }
	void SetCustomerCode(std::wstring customerCode) { mCustomerCode = customerCode; }

	std::wstring GetCreationDate() { return mCreationDate; }
	void SetCreationDate(std::wstring creationDate) { mCreationDate = creationDate; }
	
protected:
	int mId;
	int mRemoteId;
	std::wstring mCustomerName;
	std::wstring mCustomerCode;
	std::wstring mCreationDate;
	std::wstring mNotes;
};

