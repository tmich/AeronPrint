#include "MyFrame.h"

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(ID_Hello, MyFrame::OnHello)
	EVT_MENU(wxID_EXIT, MyFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
	EVT_LIST_ITEM_ACTIVATED(LIST_CTRL, MyFrame::OnSelectedItem)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString & title, const wxPoint & pos, const wxSize & size)
	: wxFrame(nullptr, wxID_ANY, title, pos, size)
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

	wxButton *btnCheckNew = new wxButton(m_panel, wxID_ANY, wxT("Controlla"), wxPoint(350, 0),wxSize(100, 100));
	
	/*btnCheckNew->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {
		OnCheckNew(event);
	});*/

	btnCheckNew->Bind(wxEVT_BUTTON, &MyFrame::OnCheckNew, this);
	btnCheckNew->Show();

	OrderService orderService;

	try
	{
		auto orders = orderService.GetAllOrders();
		updateList(orders);
	}
	catch (const DatabaseException& db_exception)
	{
		std::wstringstream ws;
		ws << L"Errore nel database: " << db_exception.what();

		SetStatusText(ws.str());
	}
	catch (const std::exception& exc)
	{
		wxMessageBox(exc.what(), L"Errore");
	}
	
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

void MyFrame::OnSelectedItem(wxListEvent& event)
{
	int idx = event.GetIndex();

	try
	{
		int orderId = atoi(event.GetText());

		// TODO: stampa
		wxMessageBox(event.GetText());

		OrderService orderService;
		Order order = orderService.GetOrderById(orderId);
		order.IsRead(true);
		orderService.Save(order);
		m_listCtrl->SetItemBackgroundColour(idx, wxColour(*wxWHITE));
		event.Veto();
	}
	catch (const DatabaseException& db_exception)
	{
		std::wstringstream ws;
		ws << L"Errore nel database: " << db_exception.what();

		SetStatusText(ws.str());
	}
	catch (const std::exception& exc)
	{
		SetStatusText(exc.what());
	}
}

void MyFrame::OnCheckNew(wxCommandEvent &)
{
	SetStatusText("Connessione in corso...");

	try
	{
		OrderService orderService;
		orderService.CheckNewOrders();

		auto orders = orderService.GetAllOrders();
		updateList(orders);

		SetStatusText("Pronto.");
	}
	catch (const ConnectionException&)
	{
		SetStatusText("Impossibile connettersi al server remoto.");
	}
	catch (const DatabaseException& db_exception)
	{
		std::wstringstream ws;
		ws << L"Errore nel database: " << db_exception.what();

		SetStatusText(ws.str());
	}
	catch (const std::exception& exc)
	{
		SetStatusText(exc.what());
	}
	
}

void MyFrame::updateList(const vector<Order>& orders)
{
	m_listCtrl->ClearAll();

	m_listCtrl->AppendColumn(wxT("N"));
	m_listCtrl->AppendColumn(wxT("Cliente"));
	m_listCtrl->SetColumnWidth(0, 30);
	m_listCtrl->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
	m_listCtrl->Show();

	for each (Order o in orders)
	{
		// col 1: Id
		wxListItem listItem;
		listItem.SetId(m_listCtrl->GetItemCount());
		if (! o.IsRead())
		{
			listItem.SetBackgroundColour(wxColour(*wxGREEN));
		}
		listItem.SetText(std::to_wstring(o.GetId()));
		long list_index = m_listCtrl->InsertItem(listItem);

		// col 2: Cliente
		m_listCtrl->SetItem(list_index, 1, o.GetCustomerName());
	}
}

void MyFrame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}