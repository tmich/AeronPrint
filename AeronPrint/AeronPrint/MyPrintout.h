#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "MyApp.h"

// Defines a new printout class to print our document
class MyPrintout : public wxPrintout
{
public:
	MyPrintout(wxFrame * frame, const wxString &title = wxT("My printout"));

	virtual bool OnPrintPage(int page) wxOVERRIDE;
	virtual bool HasPage(int page) wxOVERRIDE;
	virtual bool OnBeginDocument(int startPage, int endPage) wxOVERRIDE;
	virtual void GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo) wxOVERRIDE;

	void DrawPageOne();
	void DrawPageTwo();

	// Writes a header on a page. Margin units are in millimetres.
	bool WritePageHeader(wxPrintout *printout, wxDC *dc, const wxString& text, float mmToLogical);

private:
	wxFrame *m_frame;
};