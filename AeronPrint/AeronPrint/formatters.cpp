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

	txt << "<h2>" << m_order.GetCustomerName() << " [" << m_order.GetCustomerCode() << "]" << "</h2>";
	txt << "<h5> Ordine inviato il " << date << " alle " << hour << "</h5>";
	txt << "<table border=\"1\" width=\"100%\">";
	txt << "<tr><td><font size=\"4\"><b>Qta</b></font></td><td><font size=\"4\"><b>Cod</b></font></td><td><font size=\"4\"><b>Articolo</b></font></td></tr>";
	for (auto & item : m_order.Items)
	{
		txt << "<tr>";
		txt << "<td width=\"5%\"><font size=\"4\">" << item.GetQty() << "</font></td>";
		txt << "<td width=\"10%\"><font size=\"4\">[" << item.GetCode() << "]</font></td>";
		txt << "<td width=\"55%\"><font size=\"4\">" << item.GetName() << "</font></td>";
		txt << "</tr>";
		if (item.GetNotes() != L"")
		{
			txt << "<tr><td></td><td></td><td><font size=\"3\"<i><b>NB:</b>" << item.GetNotes() << "</i></font></td></tr>";
		}
	}
	txt << "</table>";
	return txt.str();
}
