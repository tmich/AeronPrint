#include "MyApp.h"
#include "MyFrame.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	// call the base class initialization method, currently it only parses a
	// few common command-line options but it could be do more in the future
	if (!wxApp::OnInit())
		return false;

	MyFrame *frame = new MyFrame("Hello World", wxPoint(50, 50), wxSize(450, 340));
	frame->Show(true);
	return true;
}

MyApp::MyApp()
{
}
