#pragma once
#include "wx/html/htmprint.h"

class MyHtmlEasyPrinting : public wxHtmlEasyPrinting
{
public:
	MyHtmlEasyPrinting(const wxString& name, wxWindow * parent);
	virtual ~MyHtmlEasyPrinting();

	bool MyHtmlEasyPrinting::DoPrint(wxHtmlPrintout *printout)
	{
		wxPrintDialogData printDialogData(*GetPrintData());
		wxPrinter printer(&printDialogData);

		if (!printer.Print(m_ParentWindow, printout, false))
		{
			return false;
		}

		(*GetPrintData()) = printer.GetPrintDialogData().GetPrintData();
		return true;
	}
private:
	wxWindow * m_ParentWindow;
};

