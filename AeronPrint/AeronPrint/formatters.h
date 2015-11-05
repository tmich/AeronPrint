#pragma once
#include "Order.h"
#include <string>
#include <vector>

class HtmlFormatter
{
public:
	virtual std::wstring ToHtml() = 0;
};

class OrderHtmlFormatter : public HtmlFormatter
{
public:
	OrderHtmlFormatter(const Order& order);
	~OrderHtmlFormatter();
	virtual std::wstring ToHtml();
protected:
	Order m_order;
};

