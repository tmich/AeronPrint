#include "Order.h"



Order::Order(const Order & rhs)
{
	mId = rhs.mId;
	this->mCreationDate = rhs.mCreationDate;
	this->mCustomerCode = rhs.mCustomerCode;
	this->mCustomerName = rhs.mCustomerName;
	this->mNotes = rhs.mNotes;
	this->mRead = rhs.mRead;
	this->mRemoteId = rhs.mRemoteId;
	this->Items.assign(rhs.Items.begin(), rhs.Items.end());
}

Order::Order()
{
	mId = 0;
	mRemoteId = 0;
	mRead = false;
}


Order::~Order()
{
}

OrderItem::OrderItem()
{
	mId = 0;
}
