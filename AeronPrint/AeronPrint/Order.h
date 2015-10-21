#pragma once
#include <string>

class Order
{
public:
	Order();
	~Order();

	int GetId() { return mId; }
	void SetId(int id) { mId = id; }

	std::string GetCustomerName() { return mCustomerName; }
	void SetCustomerName(std::string customerName) { mCustomerName = customerName; }

	std::string GetCustomerCode() { return mCustomerCode; }
	void SetCustomerCode(std::string customerCode) { mCustomerCode = customerCode; }

	std::string GetNotes() { return mNotes; }
	void SetNotes(std::string notes) { mNotes = notes; }
	
protected:
	int mId;
	std::string mCustomerName;
	std::string mCustomerCode;
	std::string mCreationDate;
	std::string mNotes;
};

