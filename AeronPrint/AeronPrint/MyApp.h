#pragma once
// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include "orders_controller.h"

class MyApp : public wxApp
{
public:
	MyApp();
	virtual bool OnInit() wxOVERRIDE;
	virtual int OnExit() wxOVERRIDE;
	
private:
	/*OrdersView * ordersView;*/
	OrdersController * ordersController;

	wxDECLARE_NO_COPY_CLASS(MyApp);
};
DECLARE_APP(MyApp);

