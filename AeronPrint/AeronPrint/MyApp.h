#pragma once

// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <ctype.h>
#include "wx/metafile.h"

#include "wx/image.h"
#include "wx/accel.h"
#include "MyFrame.h"

#if wxUSE_GRAPHICS_CONTEXT
	#include "wx/graphics.h"
#endif

#ifndef wxHAS_IMAGES_IN_RESOURCES
	#include "../sample.xpm"
#endif



class MyApp : public wxApp
{
public:
	void Draw(wxDC & dc);
	MyApp();
	virtual bool OnInit() wxOVERRIDE;
	wxFont& GetTestFont() { return m_testFont; }

	wxPrintData * GetPrintData() { return g_printData; }
	wxPageSetupDialogData * GetPageSetupDialogData() { return g_pageSetupData; }

private:
	wxFont    m_testFont;
	wxDECLARE_NO_COPY_CLASS(MyApp);

	// Global print data, to remember settings during the session
	wxPrintData *g_printData = NULL;

	// Global page setup data
	wxPageSetupDialogData* g_pageSetupData = NULL;
};
DECLARE_APP(MyApp);

