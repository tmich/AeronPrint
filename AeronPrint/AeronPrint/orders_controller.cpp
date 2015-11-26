#include "orders_controller.h"
#include "query_constraints.h"
#include "aeron_exceptions.h"
#include "order_service.h"
#include "formatters.h"
#include "utils.h"
#include <cstdlib>
#include "config.h"

OrdersController::OrdersController(OrdersView * ordersView)
{
	view_ = ordersView;

	QueryConstraints qc;
	qc.Limit(50);
	qc.Sort("id", SortConstraint::DESCENDING);

	repo = new OrderRepository(qc);
		
	try
	{
		//auto orders = orderService.GetAllOrders();
		auto orders = repo->GetAll();
		orderPaginator = new Paginator<Order>(orders, 15);

		view_->Update(orderPaginator->Page(0));
		view_->Info(Utils::s2ws("Caricamento effettuato"));
	}
	catch (const DatabaseException& db_exception)
	{
		std::wstringstream ws;
		ws << L"Errore nel database: " << db_exception.what();

		view_->Warn(ws.str());
	}
	catch (const std::exception& exc)
	{
		wxMessageBox(exc.what(), L"Errore");
	}

	// CHECKING NEW ORDERS: scheduled task
	Config cfg;
	create_timer_and_start(std::bind(&OrdersController::checkOrders, this), cfg.PollingTime() * 1000);

	// Binding
	// ***********************
	// Check new orders
	view_->btnCheckNew->Bind(wxEVT_BUTTON, [&](wxCommandEvent & event) {
		checkOrders();
	});

	// Next Page
	view_->btnNext->Bind(wxEVT_BUTTON, [=](wxCommandEvent& WXUNUSED(event)) {
		int nPage = orderPaginator->Current() + 1;
		if (nPage >= 0 && nPage < orderPaginator->Pages())
		{
			view_->Update(orderPaginator->Page(nPage));
		}
	});

	// Prev page
	view_->btnPrev->Bind(wxEVT_BUTTON, [=](wxCommandEvent& WXUNUSED(event)) {
		int nPage = orderPaginator->Current() - 1;
		if (nPage >= 0 && nPage < orderPaginator->Pages())
		{
			view_->Update(orderPaginator->Page(nPage));
		}
	});
	
	// Menu binding
	view_->Bind(wxEVT_MENU, [=](wxCommandEvent& event) {
		auto dirname = Config::CONFIG_PATH;
		std::ostringstream sstr;

		switch (event.GetId())
		{
		case ID_ToggleTask:
			if (event.IsChecked())
			{
				polling_timer->start();
				view_->Info(L"Servizio abilitato");
			}
			else
			{
				polling_timer->pause();
				view_->Info(L"Servizio disabilitato");
			}
			event.Skip();
			break;
		case wxID_PREFERENCES:
			sstr << "explorer.exe " << dirname;
			system(sstr.str().c_str());
			break;
		default:
			event.Skip();
			break;
		}
		
	});

	// PRINT SELECTED ORDERS
	view_->btnPrint->Bind(wxEVT_BUTTON, [=](wxCommandEvent& WXUNUSED(event)) {
		std::wostringstream payload;

		for (int& index : view_->OrdersList->GetSelectedItems())
		{
			int orderId = view_->OrdersList->GetItemData(index);
			Order order = repo->Get(orderId);
			OrderHtmlFormatter html(order);
			payload << html.ToHtml();
			order.IsRead(true);
			repo->Update(order);
			view_->UpdateListItem(index, order);
		}

		if (payload.str().size() > 0)
		{
			view_->Printer()->PrintText(payload.str());
		}
	});

	//// LIST RIGHT CLICK
	//view_->OrdersList->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, [&](wxListEvent& event) {
	//	//popupMenu = new wxMenu();
	//	void *data = reinterpret_cast<void *>(event.GetItem().GetData());
	//	wxMenu mnu;
	//	mnu.SetClientData(data);
	//	mnu.Append(300, "Do something");
	//	mnu.Append(301, "Do something else");
	//	//mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(OrdersController::OnPopupClick), NULL, this);
	//	view_->PopupMenu(&mnu);
	//});

	// PRINT PREVIEW
	view_->OrdersList->Bind(wxEVT_LIST_ITEM_ACTIVATED, [&](wxListEvent& event) {
		int idx = event.GetIndex();

		try
		{
			int orderId = atoi(event.GetText());
			Order order = repo->Get(orderId);

			/*std::wostringstream wss;
			wss << "Ordine n. " << order.GetRemoteId();
			view_->Printer()->SetName(wss.str());
			wss << " (@PAGENUM@/@PAGESCNT@)<hr>";
			view_->Printer()->SetHeader(wss.str(), wxPAGE_ALL);*/

			//view_->PrintPreview(order);
			OrderHtmlFormatter html(order);
			auto isRead = view_->Printer()->PreviewText(html.ToHtml());

			order.IsRead(true);
			repo->Update(order);
			view_->UpdateListItem(idx, order);
			event.Veto();
		}
		catch (const DatabaseException& db_exception)
		{
			std::wstringstream ws;
			ws << L"Errore nel database: " << db_exception.what();

			view_->Warn(ws.str());
		}
		catch (const std::exception& exc)
		{
			view_->Warn(Utils::s2ws(exc.what()));
		}
	});

	view_->Show(true);
}

void OrdersController::checkOrders()
{
	view_->Info(L"Connessione in corso...");

	try
	{
		polling_timer->pause();

		OrderService orderService;
		auto new_orders = orderService.CheckNewOrders();
		int sz = new_orders.size();

		if (sz > 0)
		{
			repo->AddAll(new_orders);
			auto orders = repo->GetAll();
			orderPaginator = new Paginator<Order>(orders, 15);
			view_->Update(orderPaginator->Page(0));
			view_->NotifyNewOrders(sz);
		}
		else
		{
			view_->Info(L"Dati aggiornati");
		}

		polling_timer->start();
	}
	catch (const ConnectionException&)
	{
		view_->Warn(L"Impossibile connettersi al server remoto.");
	}
	catch (const DatabaseException& db_exception)
	{
		std::wstringstream ws;
		ws << L"Errore nel database: " << db_exception.what();

		view_->Warn(ws.str());
	}
	catch (const std::exception& exc)
	{
		view_->Warn(Utils::s2ws(exc.what()));
	}
}

OrdersController::~OrdersController()
{
	polling_timer->stop();
	//delete polling_timer;
}

