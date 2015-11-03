#include "MyApp.h"
#include "orders_controller.h"
#include "orders_view.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	// call the base class initialization method, currently it only parses a
	// few common command-line options but it could be do more in the future
	if (!wxApp::OnInit())
		return false;

	OrdersView *frame = new OrdersView("AEG 2000");
	OrdersController * ctrl = new OrdersController(frame);
	//frame->Centre(wxBOTH);
	//frame->Show(true);

	return true;
}

MyApp::MyApp()
{
}
