#include "MyApp.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	// call the base class initialization method, currently it only parses a
	// few common command-line options but it could be do more in the future
	if (!wxApp::OnInit())
		return false;

	g_pageSetupData = new wxPageSetupDialogData;

	// copy over initial paper size from print record
	(*g_pageSetupData) = *g_printData;

	// Set some initial page margins in mm.
	g_pageSetupData->SetMarginTopLeft(wxPoint(15, 15));
	g_pageSetupData->SetMarginBottomRight(wxPoint(15, 15));

	MyFrame *frame = new MyFrame("AEG 2000", wxPoint(50, 50), wxSize(450, 340));
	frame->Centre(wxBOTH);
	frame->Show(true);
	return true;
}

void MyApp::Draw(wxDC& dc)
{
	// This routine just draws a bunch of random stuff on the screen so that we
	// can check that different types of object are being drawn consistently
	// between the screen image, the print preview image (at various zoom
	// levels), and the printed page.
	dc.SetBackground(*wxWHITE_BRUSH);
	// dc.Clear();
	dc.SetFont(m_testFont);

	// dc.SetBackgroundMode(wxTRANSPARENT);

	dc.SetPen(*wxBLACK_PEN);
	dc.SetBrush(*wxLIGHT_GREY_BRUSH);

	dc.DrawRectangle(0, 0, 230, 350);
	dc.DrawLine(0, 0, 229, 349);
	dc.DrawLine(229, 0, 0, 349);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);

	dc.SetBrush(*wxCYAN_BRUSH);
	dc.SetPen(*wxRED_PEN);

	dc.DrawRoundedRectangle(0, 20, 200, 80, 20);

	dc.DrawText(wxT("Rectangle 200 by 80"), 40, 40);

	dc.SetPen(wxPen(*wxBLACK, 0, wxPENSTYLE_DOT_DASH));
	dc.DrawEllipse(50, 140, 100, 50);
	dc.SetPen(*wxRED_PEN);

	dc.DrawText(wxT("Test message: this is in 10 point text"), 10, 180);

#if wxUSE_UNICODE
	const char *test = "Hebrew    ???? -- Japanese (???)";
	wxString tmp = wxConvUTF8.cMB2WC(test);
	dc.DrawText(tmp, 10, 200);
#endif

	wxPoint points[5];
	points[0].x = 0;
	points[0].y = 0;
	points[1].x = 20;
	points[1].y = 0;
	points[2].x = 20;
	points[2].y = 20;
	points[3].x = 10;
	points[3].y = 20;
	points[4].x = 10;
	points[4].y = -20;
	dc.DrawPolygon(5, points, 20, 250, wxODDEVEN_RULE);
	dc.DrawPolygon(5, points, 50, 250, wxWINDING_RULE);

	dc.DrawArc(20, 330, 40, 300, 20, 300);
	{
		wxDCBrushChanger changeBrush(dc, *wxTRANSPARENT_BRUSH);
		dc.DrawArc(60, 330, 80, 300, 60, 300);
	}

	dc.DrawEllipticArc(80, 250, 60, 30, 0.0, 270.0);

	points[0].x = 150;
	points[0].y = 250;
	points[1].x = 180;
	points[1].y = 250;
	points[2].x = 180;
	points[2].y = 220;
	points[3].x = 200;
	points[3].y = 220;
	dc.DrawSpline(4, points);

	wxString str;
	int i = 0;
	str.Printf(wxT("---- Text at angle %d ----"), i);
	dc.DrawRotatedText(str, 100, 300, i);

	i = 90;
	str.Printf(wxT("---- Text at angle %d ----"), i);
	dc.DrawRotatedText(str, 100, 300, i);

	wxIcon my_icon = wxICON(sample);

	dc.DrawIcon(my_icon, 100, 100);

#if wxUSE_GRAPHICS_CONTEXT
	wxGraphicsContext *gc = NULL;

	wxPrinterDC *printer_dc = wxDynamicCast(&dc, wxPrinterDC);
	if (printer_dc)
		gc = wxGraphicsContext::Create(*printer_dc);

	wxWindowDC *window_dc = wxDynamicCast(&dc, wxWindowDC);
	if (window_dc)
		gc = wxGraphicsContext::Create(*window_dc);

#ifdef __WXMSW__
	wxEnhMetaFileDC *emf_dc = wxDynamicCast(&dc, wxEnhMetaFileDC);
	if (emf_dc)
		gc = wxGraphicsContext::Create(*emf_dc);
#endif

	if (gc)
	{
		// make a path that contains a circle and some lines, centered at 100,100
		gc->SetPen(*wxRED_PEN);

		wxGraphicsPath path = gc->CreatePath();
		path.AddCircle(50.0, 50.0, 50.0);
		path.MoveToPoint(0.0, 50.0);
		path.AddLineToPoint(100.0, 50.0);
		path.MoveToPoint(50.0, 0.0);
		path.AddLineToPoint(50.0, 100.0);
		path.CloseSubpath();
		path.AddRectangle(25.0, 25.0, 50.0, 50.0);

		gc->StrokePath(path);

		// draw some text
		wxString text("Text by wxGraphicsContext");
		gc->SetFont(m_testFont, *wxBLACK);
		gc->DrawText(text, 25.0, 60.0);

		// draw rectangle around the text
		double w, h, d, el;
		gc->GetTextExtent(text, &w, &h, &d, &el);
		gc->SetPen(*wxBLACK_PEN);
		gc->DrawRectangle(25.0, 60.0, w, h);

		delete gc;
	}
#endif
}

MyApp::MyApp()
{
}
