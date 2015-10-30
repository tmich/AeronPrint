#pragma once

// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "wx/html/htmprint.h"
#include "OrderListCtrl.h"
#include "order_service.h"
#include "MyApp.h"
#include <vector>

class MyFrame : public wxFrame
{
public:
	MyFrame(const wxString & title, const wxPoint & pos, const wxSize & size);
	//void OnPreviewFrameModalityKind(wxCommandEvent& event);
	~MyFrame();

	OrderListCtrl *m_listCtrl;
	wxPanel *m_panel;
private:
	void OnHello(wxCommandEvent&);
	void OnExit(wxCommandEvent&);
	void OnAbout(wxCommandEvent&);
	void OnSelectedItem(wxListEvent&);
	void OnCheckNew(wxCommandEvent&);
	void OnPrintPreview(wxCommandEvent&);
	
	void updateList(const vector<Order>& orders);

	wxMenuItem * mnuPrint;
	wxHtmlEasyPrinting * m_Prn;
	
	wxDECLARE_NO_COPY_CLASS(MyFrame);
	wxDECLARE_EVENT_TABLE();
};

enum
{
	ID_Hello = 1
};
