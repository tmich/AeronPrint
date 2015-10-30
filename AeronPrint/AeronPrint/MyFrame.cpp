#include "MyFrame.h"

MyFrame::MyFrame(const wxString & title, const wxPoint & pos, const wxSize & size) 
	: wxFrame(nullptr, wxID_ANY, title, pos, size)
{
	wxMenu *menuFile = new wxMenu;

	mnuPrint = new wxMenuItem(menuFile, wxID_PREVIEW, "&Anteprima di stampa\tCtrl-P", "Antemprima di Stampa");
	mnuPrint->Enable(false);
	menuFile->Append(mnuPrint);
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");

	this->Centre(wxBOTH);
	SetMenuBar(menuBar);
	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");

	m_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(450, 340));
	m_listCtrl = new OrderListCtrl(m_panel, LIST_CTRL,
		wxDefaultPosition, wxSize(450, 300),
		wxLC_REPORT | wxBORDER_THEME | wxLC_EDIT_LABELS);

	wxButton *btnCheckNew = new wxButton(m_panel, wxID_ANY, wxT("Controlla"), wxPoint(0, 350),wxSize(100, 30));
	
	/*btnCheckNew->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {
		OnCheckNew(event);
	});*/
	
	// Stampa
	m_Prn = new wxHtmlEasyPrinting(_("Stampa Ordine"), this);
	
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

		OrderService orderService;
		Order order = orderService.GetOrderById(orderId);
		
		std::wostringstream wss, txt;
		wss << "Ordine n. " << orderId;
		m_Prn->SetName(wss.str());
		wss << " (@PAGENUM@/@PAGESCNT@)<hr>";
		m_Prn->SetHeader(wss.str(), wxPAGE_ALL);

		// data: 2015-10-22 11:40:35.146612

		std::wstring complete_date, day, month, year, hh, mm;
		complete_date = order.GetCreationDate();
		year = complete_date.substr(0, 4);
		month = complete_date.substr(5, 2);
		day = complete_date.substr(8, 2);
		hh = complete_date.substr(11, 2);
		mm = complete_date.substr(14, 2);

		txt << "<h2>" << order.GetCustomerName() << " [" << order.GetCustomerCode() << "]" << "</h2>";
		txt << "<h5> Ordine inviato il " << day << "-" << month << "-" << year << " alle " << hh << ":" << mm << "</h5>";
		txt << "<table border=\"1\" width=\"100%\">";
		txt << "<tr><td><font size=\"4\"><b>Qta</b></font></td><td><font size=\"4\"><b>Cod</b></font></td><td><font size=\"4\"><b>Articolo</b></font></td></tr>";
		for (auto & item : order.Items)
		{
			txt << "<tr>";
			txt << "<td width=\"5%\"><font size=\"4\">" << item.GetQty() << "</font></td>";
			txt << "<td width=\"10%\"><font size=\"4\">[" << item.GetCode() << "]</font></td>";
			txt << "<td width=\"55%\"><font size=\"4\">" << item.GetName() << "</font></td>";
			txt << "</tr>";
			if (item.GetNotes() != "")
			{
				txt << "<tr><td></td><td></td><td><font size=\"3\"<i><b>NB:</b>" << item.GetNotes() << "</i></font></td></tr>";
			}
		}
		txt << "</table>";
		m_Prn->PreviewText(txt.str());

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

void MyFrame::OnPrintPreview(wxCommandEvent& WXUNUSED(event))
{
	m_Prn->PreviewText("<h1>Prova</h1>");
}

void MyFrame::updateList(const vector<Order>& orders)
{
	m_listCtrl->ClearAll();

	// Col 1: Id locale
	m_listCtrl->AppendColumn(wxT("N"));
	m_listCtrl->SetColumnWidth(0, 30);

	// Col 2: nome cliente
	m_listCtrl->AppendColumn(wxT("Cliente"));
	m_listCtrl->SetColumnWidth(1, 250);
	
	// Col 3: data creazione
	m_listCtrl->AppendColumn(wxT("Data"));
	m_listCtrl->SetColumnWidth(2, 80);

	// Col 4: Id remoto
	m_listCtrl->AppendColumn(wxT("Id"));
	m_listCtrl->SetColumnWidth(3, 30);

	
	m_listCtrl->Show();

	// iterator
	for (auto & iit = orders.crbegin(); iit != orders.crend(); iit++)
	{
		auto o = *iit;

		// col 1: Id
		wxListItem listItem;
		listItem.SetId(m_listCtrl->GetItemCount());
		if (! o.IsRead())
		{
			listItem.SetBackgroundColour(wxColour(*wxGREEN));
		}
		listItem.SetText(std::to_wstring(o.GetId()));
		long list_index = m_listCtrl->InsertItem(listItem);

		// col 2: nome cliente
		m_listCtrl->SetItem(list_index, 1, o.GetCustomerName());

		// Col 3: data creazione
		m_listCtrl->SetItem(list_index, 2, o.GetCreationDate());

		// Col 4: id remoto
		std::wstringstream s;
		s << o.GetRemoteId();
		m_listCtrl->SetItem(list_index, 3, wxString(s.str()));
	}
}

MyFrame::~MyFrame()
{
	wxDELETE(m_Prn);
}

void MyFrame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(ID_Hello, MyFrame::OnHello)
	EVT_MENU(wxID_PREVIEW, MyFrame::OnPrintPreview)
	EVT_MENU(wxID_EXIT, MyFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
	EVT_LIST_ITEM_ACTIVATED(LIST_CTRL, MyFrame::OnSelectedItem)
wxEND_EVENT_TABLE()