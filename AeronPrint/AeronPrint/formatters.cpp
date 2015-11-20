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

	txt << "<p>Ordine n. " << m_order.GetRemoteId() << "</p>";
	txt << "<h2>" << m_order.GetCustomerName() << " [" << m_order.GetCustomerCode() << "]" << "</h2>";
	txt << "<b> Data invio: " << date << " alle " << hour << "</b>";
	txt << "<table border=\"1\" width=\"100%\">";
	txt << "<tr><td><b>Qta</b></td><td><b>Cod</b></td><td><b>Articolo</b></td></tr>";
	for (auto & item : m_order.Items)
	{
		txt << "<tr>";
		txt << "<td width=\"5%\">" << item.GetQty() << "</td>";
		txt << "<td width=\"10%\">[" << item.GetCode() << "]</td>";
		txt << "<td width=\"55%\">" << item.GetName() << "</td>";
		txt << "</tr>";
		if (item.GetNotes() != L"")
		{
			txt << "<tr><td>&#160;</td><td><font size=\"5\">&#160;&#160;&#10159;</font></td><td><b>Note:</b>" << item.GetNotes() << "</td></tr>";
		}
	}
	txt << "</table><br /><hr />";
	return txt.str();
}
