#include "MyFrame.h"

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(ID_Hello, MyFrame::OnHello)
	EVT_MENU(wxID_EXIT, MyFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
		"Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);
	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");

	m_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200,200));

	m_listCtrl = new OrderListCtrl(m_panel, LIST_CTRL,
		wxDefaultPosition, wxDefaultSize,
		wxLC_REPORT | wxBORDER_THEME | wxLC_EDIT_LABELS);

	m_listCtrl->AppendColumn(wxT("N"));
	m_listCtrl->AppendColumn(wxT("Cliente"));
	m_listCtrl->SetColumnWidth(0, 30);
	m_listCtrl->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
	m_listCtrl->Show();

	long list_index = m_listCtrl->InsertItem(m_listCtrl->GetItemCount(), wxT("1"));
	m_listCtrl->SetItem(list_index, 1, wxT("Bappo SrL"));

	list_index = m_listCtrl->InsertItem(m_listCtrl->GetItemCount(), wxT("2"));
	m_listCtrl->SetItem(list_index, 1, wxT("L'Officina del Pane SRL"));
	
}
void MyFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("This is a wxWidgets' Hello world sample",
		"About Hello World", wxOK | wxICON_INFORMATION);
}
void MyFrame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}