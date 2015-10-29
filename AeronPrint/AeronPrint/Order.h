#pragma once
#include <string>
#include <vector>

class OrderItem
{
public:
	OrderItem();

	int GetId() { return mId; }
	void SetId(int id) { mId = id; }

	int GetQty() { return mQty; }
	void SetQty(int qty) { mQty = qty; }

	std::wstring GetCode() { return mCode; }
	void SetCode(std::wstring code) { mCode = code; }

	std::wstring GetName() { return mName; }
	void SetName(std::wstring name) { mName = name; }

	std::wstring GetNotes() { return mNotes; }
	void SetNotes(std::wstring notes) { mNotes = notes; }

	int GetRemoteId() { return mRemoteId; }
	void SetRemoteId(int id) { mRemoteId = id; }

	int GetOrderId() { return mOrderId; }
	void SetOrderId(int orderId) { mOrderId = orderId; }


protected:
	int mId;
	int mRemoteId;
	int mQty;
	int mOrderId;
	std::wstring mCode;
	std::wstring mName;
	std::wstring mNotes;
};

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

	bool IsRead() { return mRead; }
	void IsRead(bool isRead) { mRead = isRead; }

	std::vector<OrderItem> Items;
	
protected:
	int mId;
	int mRemoteId;
	std::wstring mCustomerName;
	std::wstring mCustomerCode;
	std::wstring mCreationDate;
	std::wstring mNotes;
	bool mRead;
};

