#include "formatters.h"
#include <sstream>
#include "utils.h"


OrderHtmlFormatter::OrderHtmlFormatter(const Order& order) : m_order{order}
{
}


OrderHtmlFormatter::~OrderHtmlFormatter()
{
}

std::wstring OrderHtmlFormatter::ToHtml()
{
	std::wostringstream txt;

	std::wstring date, hour;
	Utils::format_date(m_order.GetCreationDate(), date, hour);

	txt << "<h2>Ordine n. " << m_order.GetRemoteId() << "</h2><hr />";
	txt << "<h3>" << " [" << m_order.GetCustomerCode() << "]" << m_order.GetCustomerName() << "</h3>";
	txt << "<b> Data invio: " << date << " alle " << hour << "</b>";
	txt << "<table border=\"1\" width=\"100%\">";
	txt << "<tr><td width=\"10%\"><b>Cod</b></td><td width=\"75%\"><b>Articolo</b></td><td><b>Qta</b></td></tr>";
	for (auto & item : m_order.Items)
	{
		txt << "<tr>";
		txt << "<td>[" << item.GetCode() << "]</td>";
		txt << "<td>" << item.GetName() << "</td>";
		txt << "<td>" << item.GetQty() << "</td>";
		txt << "</tr>";
		if (item.GetNotes() != L"")
		{
			txt << "<tr><td><font size=\"5\">&#160;&#160;&#10159;</font></td><td><b>Note:</b>" << item.GetNotes() << "</td><td>&#160;</td></tr>";
		}
	}
	txt << "</table><br /><hr />";
	return txt.str();
}
