#pragma once
#include <string>
#include <vector>

class OrderItem
{
public:
	OrderItem();

	int GetId() const { return mId; }
	void SetId(int id) { mId = id; }

	int GetQty() const { return mQty; }
	void SetQty(int qty) { mQty = qty; }

	std::wstring GetCode() const { return mCode; }
	void SetCode(std::wstring code) { mCode = code; }

	std::wstring GetName() const { return mName; }
	void SetName(std::wstring name) { mName = name; }

	std::wstring GetNotes() const { return mNotes; }
	void SetNotes(std::wstring notes) { mNotes = notes; }

	int GetRemoteId() const { return mRemoteId; }
	void SetRemoteId(int id) { mRemoteId = id; }

	int GetOrderId() const { return mOrderId; }
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
	Order(const Order&);

	Order();
	~Order();

	int GetId() const { return mId; }
	void SetId(int id) { mId = id; }

	int GetRemoteId() const { return mRemoteId; }
	void SetRemoteId(int id) { mRemoteId = id; }

	std::wstring GetCustomerName() const { return mCustomerName; }
	void SetCustomerName(std::wstring customerName) { mCustomerName = customerName; }

	std::wstring GetCustomerCode() const { return mCustomerCode; }
	void SetCustomerCode(std::wstring customerCode) { mCustomerCode = customerCode; }

	std::wstring GetCreationDate() const { return mCreationDate; }
	void SetCreationDate(std::wstring creationDate) { mCreationDate = creationDate; }

	bool IsRead() const { return mRead; }
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

