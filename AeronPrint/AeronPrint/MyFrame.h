#pragma once

// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "OrderListCtrl.h"

class MyFrame : public wxFrame
{
public:
	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

	OrderListCtrl *m_listCtrl;
	wxPanel *m_panel;
private:
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	
	wxDECLARE_NO_COPY_CLASS(MyFrame);
	wxDECLARE_EVENT_TABLE();
};

enum
{
	ID_Hello = 1
};
