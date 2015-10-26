#pragma once

// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <ctype.h>
#include "wx/metafile.h"
#include "wx/print.h"
#include "wx/printdlg.h"
#include "wx/image.h"
#include "wx/accel.h"
#include "OrderListCtrl.h"
#include "order_service.h"
#include "MyPrintout.h"
#include "MyApp.h"
#include <vector>

class MyCanvas;

class MyFrame : public wxFrame
{
public:
	MyFrame(const wxString & title, const wxPoint & pos, const wxSize & size);
	void OnPreviewFrameModalityKind(wxCommandEvent& event);

	OrderListCtrl *m_listCtrl;
	wxPanel *m_panel;
private:
	void OnHello(wxCommandEvent&);
	void OnExit(wxCommandEvent&);
	void OnAbout(wxCommandEvent&);
	void OnSelectedItem(wxListEvent&);
	void OnCheckNew(wxCommandEvent&);
	void OnPrintPreview(wxCommandEvent&);
	
	MyCanvas * m_canvas = nullptr;
	wxPreviewFrameModalityKind m_previewModality;

	void updateList(const vector<Order>& orders);
	
	wxDECLARE_NO_COPY_CLASS(MyFrame);
	wxDECLARE_EVENT_TABLE();
};

// Define a new white canvas
class MyCanvas : public wxScrolledWindow
{
public:
	MyCanvas(MyFrame *frame, const wxPoint& pos, const wxSize& size, long style = wxRETAINED);

	//void OnPaint(wxPaintEvent& evt);
	virtual void OnDraw(wxDC& dc) wxOVERRIDE;

private:
	wxDECLARE_EVENT_TABLE();
};

enum
{
	ID_Hello = 1
};
