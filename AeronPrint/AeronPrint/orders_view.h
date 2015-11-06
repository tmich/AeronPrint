#pragma once

// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "wx/html/htmprint.h"
#include "my_printing.h"
#include "OrderListCtrl.h"
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
	wxButton * btnPrint;

	wxMenuItem * mnuItemToggleTask;
	
	void NotifyNewOrders(size_t n);
	void Warn(std::wstring msg);
	void Info(std::wstring msg);

	void Update(const std::vector<Order>& orders);
	/*bool PrintPreview(const Order& order);
	bool Print(const Order& order);*/
	void UpdateListItem(int idx, const Order & order);

	MyHtmlEasyPrinting * Printer();
	wxCommandEvent * my_event;
private:
	void OnHello(wxCommandEvent&);
	void OnToggleTask(wxCommandEvent& event);
	void OnExit(wxCommandEvent&);
	void OnAbout(wxCommandEvent&);
	
	void updateList(const std::vector<Order>& orders);

	wxFont * defaultFont;

	wxMenuItem * mnuPrint;
	MyHtmlEasyPrinting * m_Prn;

	
	
	wxDECLARE_NO_COPY_CLASS(OrdersView);
	wxDECLARE_EVENT_TABLE();
};

//wxDEFINE_EVENT(EVT_TASK_TOGGLE, wxCommandEvent);

enum
{
	ID_Hello = 1,
	ID_ToggleTask
};
