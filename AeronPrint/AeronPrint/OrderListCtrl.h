#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/listctrl.h>
#include <vector>

class OrderListCtrl : public wxListCtrl
{
public:
	OrderListCtrl(wxWindow *parent,
		const wxWindowID id,
		const wxPoint& pos,
		const wxSize& size,
		long style)
		: wxListCtrl(parent, id, pos, size, style)
	{
		m_updated = -1;

	}

	std::vector<int> GetSelectedItems();

	//void OnActivated(wxListEvent& event);
private:
	long m_updated;

	wxDECLARE_NO_COPY_CLASS(OrderListCtrl);
	wxDECLARE_EVENT_TABLE();
};

enum
{
	LIST_CTRL = 1000
};