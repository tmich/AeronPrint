#include "orders_controller.h"
#include "query_constraints.h"
#include "aeron_exceptions.h"
#include "order_service.h"
#include "formatters.h"
#include "utils.h"

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

	// Bind

	view_->btnCheckNew->Bind(wxEVT_BUTTON, [&](wxCommandEvent & event) {
		view_->Info(L"Connessione in corso...");

		try
		{
			OrderService orderService;
			auto new_orders = orderService.CheckNewOrders();

			if (new_orders.size() > 0)
			{
				std::ostringstream ss;
				ss << "Trovati " << new_orders.size() << " nuovi ordini";
				view_->Info(Utils::s2ws(ss.str()));

				repo->AddAll(new_orders);
				auto orders = repo->GetAll();
				orderPaginator = new Paginator<Order>(orders, 15);
				view_->Update(orderPaginator->Page(0));
			}
			else
			{
				view_->Info(L"Dati aggiornati");
			}
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
	});

	view_->btnNext->Bind(wxEVT_BUTTON, [=](wxCommandEvent& WXUNUSED(event)) {
		int nPage = orderPaginator->Current() + 1;
		if (nPage >= 0 && nPage < orderPaginator->Pages())
		{
			view_->Update(orderPaginator->Page(nPage));
		}
	});

	view_->btnPrev->Bind(wxEVT_BUTTON, [=](wxCommandEvent& WXUNUSED(event)) {
		int nPage = orderPaginator->Current() - 1;
		if (nPage >= 0 && nPage < orderPaginator->Pages())
		{
			view_->Update(orderPaginator->Page(nPage));
		}
	});
	
	// menu
	view_->Bind(wxEVT_MENU, [=](wxCommandEvent& event) {
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
		default:
			event.Skip();
			break;
		}
		
	});

	// PRINT SELECTED ORDERS
	view_->btnPrint->Bind(wxEVT_BUTTON, [=](wxCommandEvent& WXUNUSED(event)) {
		long itemIndex = -1;

		while ((itemIndex = view_->OrdersList->GetNextItem(itemIndex,
			wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND) {
			// Got the selected item index
			//wxLogDebug(OrdersList->GetItemText(itemIndex));
			int orderId = atoi(view_->OrdersList->GetItemText(itemIndex));
			Order order = repo->Get(orderId);

			OrderHtmlFormatter html(order);
			//auto isRead = view_->Print(order);
			auto isRead = view_->Printer()->PrintText(html.ToHtml());
			
			order.IsRead(isRead);
			repo->Update(order);
			view_->UpdateListItem(itemIndex, order);
		}
	});

	// PRINT PREVIEW
	view_->OrdersList->Bind(wxEVT_LIST_ITEM_ACTIVATED, [&](wxListEvent& event) {
		int idx = event.GetIndex();

		try
		{
			int orderId = atoi(event.GetText());
			Order order = repo->Get(orderId);

			std::wostringstream wss;
			wss << "Ordine n. " << order.GetId();
			view_->Printer()->SetName(wss.str());
			wss << " (@PAGENUM@/@PAGESCNT@)<hr>";
			view_->Printer()->SetHeader(wss.str(), wxPAGE_ALL);

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

	// scheduled task
	std::function<void()> callback = [&] {
		try
		{
			OrderService serv;
			auto orders = serv.CheckNewOrders();
			auto sz = orders.size();
			view_->NotifyNewOrders(sz);
			
		}
		catch (ConnectionException&)
		{
			view_->Warn(Utils::s2ws("Impossibile contattare il servizio remoto."));
		}
	};

	create_timer_and_start(callback, 10000);
}

OrdersController::~OrdersController()
{
	polling_timer->stop();
	//delete polling_timer;
}

