#pragma once

// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "wx/html/htmprint.h"
#include "OrderListCtrl.h"
#include "MyApp.h"
#include <vector>
#include "paginator.h"

class OrdersView : public wxFrame
{
public:
	OrdersView(const wxString & title);
	~OrdersView();

	OrderListCtrl * OrdersList;
	wxPanel *m_panel;

	wxButton * btnCheckNew;
	wxButton * btnNext;
	wxButton * btnPrev;

	void Update(const std::vector<Order>& orders);
	void PrintPreview(const Order& order);
	void UpdateListItem(int idx, const Order & order);
private:
	void OnHello(wxCommandEvent&);
	void OnExit(wxCommandEvent&);
	void OnAbout(wxCommandEvent&);

	void updateList(const std::vector<Order>& orders);

	wxFont * defaultFont;

	wxMenuItem * mnuPrint;
	wxHtmlEasyPrinting * m_Prn;
		
	wxDECLARE_NO_COPY_CLASS(OrdersView);
	wxDECLARE_EVENT_TABLE();
};

enum
{
	ID_Hello = 1
};
