#include "orders_view.h"
#include "utils.h"
#include "formatters.h"
#include "version.h"

OrdersView::OrdersView(const wxString & title) 
	: wxFrame(nullptr, wxID_ANY, title, wxPoint(-1, -1), wxSize(700, 550), wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
	wxMenu *menuFile = new wxMenu;
	/**************
	 *** ICON  ****
	/*************/
	SetIcon(wxICON(AAAAA));

	/**************
	 ***  MENU  ***
	/*************/
	menuFile->Append(wxID_EXIT, "Esci");

	wxMenu *menuEdit = new wxMenu;
	menuEdit->Append(wxID_PREFERENCES, "Opzioni");
	mnuItemToggleTask = new wxMenuItem(menuEdit);
	mnuItemToggleTask->SetItemLabel("Online");
	mnuItemToggleTask->SetCheckable(true);
	mnuItemToggleTask->SetId(ID_ToggleTask);
	menuEdit->Append(mnuItemToggleTask);
	mnuItemToggleTask->Check(true);
	//my_event = new wxCommandEvent(EVT_TASK_TOGGLE);
		
	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT, "Informazioni");

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuEdit, "&Modifica");
	menuBar->Append(menuHelp, "&?");

	SetMenuBar(menuBar);

	/*****************
	 ***  TOOLBAR  ***
	/****************/
	/*wxToolBar *toolbar = CreateToolBar();
	wxImage::AddHandler(new wxPNGHandler);
	wxBitmap exit(wxT("exit.png"), wxBITMAP_TYPE_PNG);
	wxBitmap refresh(wxT("refresh.png"), wxBITMAP_TYPE_PNG);
	wxBitmap print(wxT("print.png"), wxBITMAP_TYPE_PNG);
	wxBitmap preview(wxT("magnifier.png"), wxBITMAP_TYPE_PNG);
	
	toolbar->AddTool(wxID_REFRESH, "Aggiorna", refresh, "Controlla l'arrivo di nuovi ordini");
	toolbar->AddTool(wxID_PREVIEW, "Visualizza", preview, "Visualizza l'ordine");
	toolbar->AddTool(wxID_PRINT, "Stampa", print, "Stampa selezione");
	toolbar->AddTool(wxID_EXIT, "Chiudi", exit, "Chiudi l'applicazione");
	toolbar->Realize();*/

	
	/*******************
	 ***  STATUSBAR  ***
	/******************/
	CreateStatusBar();
	SetStatusText("Pronti");

	defaultFont = new wxFont(wxFontInfo(10));
	defaultFont->SetFamily(wxFONTFAMILY_SWISS);
	SetFont(*defaultFont);

	wxBoxSizer * topsizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * buttonsizer = new wxBoxSizer(wxHORIZONTAL);

	m_panel = new wxPanel(this, wxID_ANY);
	
	OrdersList = new OrderListCtrl(m_panel, LIST_CTRL,
		wxPoint(-1, -1), wxDefaultSize,
		wxLC_REPORT | wxBORDER_THEME);
	
	// LIST RIGHT CLICK
	//OrdersList->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, [&](wxListEvent& event) {
	//	//popupMenu = new wxMenu();
	//	void *data = reinterpret_cast<void *>(event.GetItem().GetData());
	//	int id = reinterpret_cast<int>(data);
	//	wxMenu mnu;
	//	mnu.SetClientData(data);
	//	
	//	//mnu.SetTitle(wxString(std::to_string(id)));
	//	mnu.Append(300, "Do something");
	//	mnu.Append(301, "Do something else");
	//	//mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(OrdersController::OnPopupClick), NULL, this);
	//	PopupMenu(&mnu);
	//});

	topsizer->Add(OrdersList,
		1,            // make vertically stretchable
		wxEXPAND |    // make horizontally stretchable
		wxALL,        //   and make border all around
		10);
	
	btnCheckNew = new wxButton(m_panel, wxID_ANY, wxT("Controlla"), wxPoint(-1, -1),wxSize(100, 30));
	
	buttonsizer->Add(btnCheckNew);

	// Stampa
	//m_Prn = new wxHtmlEasyPrinting(_("Stampa Ordine"), this);
	m_Prn = new MyHtmlEasyPrinting(_("Stampa Ordine"), this);
	
	btnPrev = new wxButton(m_panel, wxID_ANY, "<<", wxPoint(-1, -1), wxSize(30, 30));
	btnNext = new wxButton(m_panel, wxID_ANY, ">>", wxPoint(-1, -1), wxSize(30, 30));
	btnPrint = new wxButton(m_panel, wxID_ANY, "Stampa", wxPoint(-1, -1), wxSize(80, 30));

	buttonsizer->Add(btnPrev);
	buttonsizer->Add(btnNext);
	buttonsizer->Add(btnPrint);
	//btnPrint->Bind(wxEVT_BUTTON, [=](wxCommandEvent& WXUNUSED(event)) {
	//	long itemIndex = -1;

	//	while ((itemIndex = OrdersList->GetNextItem(itemIndex,
	//		wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND) {
	//		// Got the selected item index
	//		wxLogDebug(OrdersList->GetItemText(itemIndex));
	//	}
	//});
	
	Centre(wxBOTH);

	topsizer->Add(buttonsizer);

	m_panel->SetSizerAndFit(topsizer);
}

void OrdersView::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void OrdersView::OnAbout(wxCommandEvent& event)
{
	Version version;
	std::ostringstream ss;
	ss << "Versione: " << version.ToString() << "\n";
	wxMessageBox(ss.str(), "Informazioni su Aeron Print", wxOK | wxICON_INFORMATION);
}

void OrdersView::updateList(const std::vector<Order>& orders)
{
	OrdersList->ClearAll();

	// Col 1: Id
	OrdersList->AppendColumn(wxT("id"));
	OrdersList->SetColumnWidth(0, 0);

	// Col 2: Numero (id remoto)
	OrdersList->AppendColumn(wxT("N°"));
	OrdersList->SetColumnWidth(1, 60);

	// Col 3: nome cliente
	OrdersList->AppendColumn(wxT("Cliente"));
	OrdersList->SetColumnWidth(2, 450);
	
	// Col 4: data creazione
	OrdersList->AppendColumn(wxT("Data"));
	OrdersList->SetColumnWidth(3, 150);

	OrdersList->Show();

	// aggiungo gli ordini alla lista
	for (auto & iit = orders.cbegin(); iit != orders.cend(); iit++)
	{
		auto o = *iit;

		// col 1: Id
		wxListItem listItem;
		listItem.SetId(OrdersList->GetItemCount());
		listItem.SetData(o.GetId());
		long list_index = OrdersList->InsertItem(listItem);

		UpdateListItem(list_index, o);
	}
}

OrdersView::~OrdersView()
{
	wxDELETE(m_Prn);
}

void OrdersView::NotifyNewOrders(size_t n)
{
	//auto clr = btnCheckNew->GetBackgroundColour();
	//auto stl = btnCheckNew->GetBackgroundStyle();

	if (n > 0)
	{
		std::string x = (n == 1 ? "o" : "i");
		std::string x2 = (n == 1 ? "e" : "i");
		std::ostringstream ss;
		ss << "Ricevut" << x << " " << n << " nuov" << x << " ordin" << x2;
		Info(Utils::s2ws(ss.str()));
		//std::ostringstream ss;
		//ss << "Ci sono " << n << " ordini da scaricare";
		//SetStatusText(ss.str());
		wxMessageBox(ss.str(), "Avviso", wxOK | wxICON_INFORMATION);
		//btnCheckNew->SetBackgroundColour(*wxRED);
	}
	//else
	//{
	//	SetStatusText("Nessun nuovo ordine");
	//	//btnCheckNew->SetBackgroundColour(wxColour(240, 240, 240));
	//	btnCheckNew->SetBackgroundStyle(wxBG_STYLE_ERASE);
	//}
}

void OrdersView::Info(std::wstring msg)
{
	SetStatusText(msg);
}

void OrdersView::Warn(std::wstring msg)
{
	SetStatusText(msg);
}

void OrdersView::Update(const std::vector<Order>& orders)
{
	updateList(orders);
}

void OrdersView::UpdateListItem(int idx, const Order & order)
{
	OrdersList->SetItemBackgroundColour(idx, wxColour(*wxWHITE));
	OrdersList->SetItemFont(idx, *defaultFont);
	
	if (!order.IsRead())
	{
		wxFont bold(*defaultFont);
		bold.SetWeight(wxFONTWEIGHT_BOLD);
		OrdersList->SetItemFont(idx, bold);
		OrdersList->SetItemBackgroundColour(idx, wxColour("CYAN"));
	}

	OrdersList->SetItem(idx, 0, std::to_wstring(order.GetId()));

	// Col 2: id remoto
	std::wstringstream s;
	s << order.GetRemoteId();
	OrdersList->SetItem(idx, 1, wxString(s.str()));

	// col 3: nome cliente
	OrdersList->SetItem(idx, 2, order.GetCustomerName());

	// Col 4: data creazione
	std::wstring date, hour;
	Utils::format_date(order.GetCreationDate(), date, hour);
	OrdersList->SetItem(idx, 3, date.append(L" ").append(hour));

}

MyHtmlEasyPrinting * OrdersView::Printer()
{
	return m_Prn;
}

void OrdersView::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}

void OrdersView::OnToggleTask(wxCommandEvent& event)
{
	wxLogDebug("OrdersView::OnToggleTask");
}
//EVT_LIST_ITEM_ACTIVATED(LIST_CTRL, OrdersView::OnSelectedItem)

wxBEGIN_EVENT_TABLE(OrdersView, wxFrame)
	EVT_MENU(ID_Hello, OrdersView::OnHello)
	EVT_MENU(wxID_EXIT, OrdersView::OnExit)
	EVT_MENU(wxID_ABOUT, OrdersView::OnAbout)
	EVT_MENU(ID_ToggleTask, OrdersView::OnToggleTask)
wxEND_EVENT_TABLE()