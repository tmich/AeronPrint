#include "order_repository.h"
#include "utils.h"

/****************************
*
*       OrderRepository
*
****************************/

OrderRepository::OrderRepository()
{
	/*sqlite::Connection db;
	db.Open(DATABASE_NAME);
	try
	{
		db.Execute("create table if not exists orders (id integer primary key, customer_code text not null, customer_name text not null, creation_date text not null, remote_id integer, read integer);");
		db.Execute("create table if not exists order_items (id integer primary key, order_id integer, code text not null, name text not null, qty integer, notes text not null, remote_id integer);");
	}
	catch (sqlite::SqliteException& exc)
	{
		throw std::exception(exc.what());
	}*/
}

OrderRepository::~OrderRepository()
{
}

void OrderRepository::Add(Order & order)
{
	sqlite::Connection db;
	db.Open(DATABASE_NAME);
	auto trn = db.BeginTransaction();
	try
	{
		addWithinTransaction(trn, order);
		trn.Commit();
	}
	catch (sqlite::SqliteException& exc)
	{
		trn.Rollback();
		throw std::exception(exc.what());
	}
}

void OrderRepository::addWithinTransaction(sqlite::Transaction & trn, Order & order)
{
	OutputDebugStringW(L"\tOrderRepository::Add(db, order)\n");
	auto customerCode = Utils::to_string(order.GetCustomerCode());
	auto customerName = Utils::to_string(order.GetCustomerName());
	auto creationDate = Utils::to_string(order.GetCreationDate());
	auto remoteId = order.GetRemoteId();

	std::wostringstream insert;
	insert << "insert into orders(customer_code, customer_name, creation_date, remote_id, read) values(" <<
		Utils::ForSql(customerCode) << ", " << Utils::ForSql(customerName) << ", " << Utils::ForSql(creationDate) << 
		", " << remoteId << ", " << order.IsRead() << ");";

	int rows_affected = trn.Execute(Utils::to_string(insert.str()).c_str());
	OutputDebugStringW(L"\t\tinsert into orders\n");
	if (rows_affected == 1)
	{
		order.SetId(trn.GetLastInsertRowid());

		OrderItemRepository itemsRepo;

		// Aggiungo gli items agli ordini
		for (auto & orderItem : order.Items)
		{
			itemsRepo.Add(trn, orderItem, order.GetId());
		}
	}
}

Order OrderRepository::Get(int id)
{
	Order order;
	
	std::stringstream select;
	select << "select id, customer_code, customer_name, creation_date, remote_id, read from orders where id = " << id << " LIMIT 1";
	
	sqlite::Connection db;
	db.Open(DATABASE_NAME);

	sqlite::Cursor crs(db, select.str().c_str());

	if (crs.Next())
	{
		order = doMakeOrder(crs);
	}

	loadItemsForOrder(order);

	return order;
}

/**
**	Occhio!!! Andava in errore perchè la chiamata a doMakeOrder(crs)
**  ==COPIAVA== il cursore.
**  Risolto modificando il parametro come parametro per riferimento.
**/
std::vector<Order> OrderRepository::GetAll()
{
	std::vector<Order> orders;
	
	sqlite::Connection db;
	db.Open(DATABASE_NAME);
	//OutputDebugString(L"\tda OrderRepository::GetAll()\n");

	std::ostringstream qry;
	qry << "select id, customer_code, customer_name, creation_date, remote_id, read from orders where 1 ";

	auto filter = queryConstraints.Filter();
	if (filter.Field != "")
	{
		qry << " and " << filter.Field << filter.Operator() << filter.Value << " ";
	}

	auto sort = queryConstraints.Sort();
	if (sort.Field != "")
	{
		qry << " order by " << sort.Field << " " << (sort.Direction == SortConstraint::ASCENDING ? " asc " : " desc ");
	}

	if (queryConstraints.Limit() > 0)
	{
		qry << " limit " << queryConstraints.Limit();
	}

	sqlite::Cursor crs(db, qry.str());
	//OutputDebugString(L"\tcarico gli ordini:\n");
	// carico gli ordini
	while (crs.Next())
	{
		//LPCWSTR c = Utils::s2ws(crs.GetText(1)).c_str();
		/*std::wostringstream wstr(L"\t");
		wstr << Utils::s2ws(crs.GetText(1));
		OutputDebugString(wstr.str().c_str());*/
		orders.push_back(doMakeOrder(crs));
	}

	for (auto & order : orders)
	{
		loadItemsForOrder(order);
	}
	

	return orders;
}

void OrderRepository::loadItemsForOrder(Order & order)
{
	OrderItemRepository itemsRepo;
	auto items = itemsRepo.GetAll(order.GetId());
	order.Items.assign(items.begin(), items.end());
}

void OrderRepository::AddAll(std::vector<Order> & orders)
{
	sqlite::Connection db;
	db.Open(DATABASE_NAME);

	auto trn = db.BeginTransaction();

	try
	{
		for (auto& order : orders)
		{
			addWithinTransaction(trn, order);
		}

		// TODO: aggiungere items

		trn.Commit();
	}
	catch (sqlite::SqliteException&)
	{
		trn.Rollback();
		throw;
	}
}

void OrderRepository::Remove(int id)
{
}

void OrderRepository::Update(Order & order)
{
	sqlite::Connection db;
	db.Open(DATABASE_NAME);
	auto trn = db.BeginTransaction();
	try
	{
		std::wostringstream update;
		update << "update orders set customer_code=" << Utils::ForSql(order.GetCustomerCode()) 
			<< ", customer_name=" << Utils::ForSql(order.GetCustomerName()) 
			<< ", creation_date=" << Utils::ForSql(order.GetCreationDate()) 
			<< ", remote_id=" << order.GetRemoteId() 
			<< ", read=" << (order.IsRead() ? "1" : "0") 
			<< " where id=" << order.GetId() << ";";
		
		int rows_affected = trn.Execute(Utils::to_string(update.str()).c_str());
		OutputDebugStringW(L"\t\tupdate orders\n");

		trn.Commit();
	}
	catch (sqlite::SqliteException& exc)
	{
		trn.Rollback();
		throw std::exception(exc.what());
	}

	/*sqlite3_open_v2(DATABASE_NAME, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
	
	sqlite3_stmt *statement;
	std::string query = "update orders set customer_code = ?, customer_name = ?, creation_date = ?, read = ? where id = ? ";
	if (sqlite3_prepare_v2(sqlite, query.c_str(), query.size(), &statement, nullptr) == SQLITE_OK)
	{
		auto customerCode = to_string(order.GetCustomerCode());
		auto customerName = to_string(order.GetCustomerName());
		auto creationDate = to_string(order.GetCreationDate());

		sqlite3_bind_text(statement, 1, customerCode.c_str(), customerCode.size(), SQLITE_STATIC);
		sqlite3_bind_text(statement, 2, customerName.c_str(), customerName.size(), SQLITE_STATIC);
		sqlite3_bind_text(statement, 3, creationDate.c_str(), creationDate.size(), SQLITE_STATIC);
		sqlite3_bind_int(statement, 4, order.IsRead());
		sqlite3_bind_int(statement, 5, order.GetId());

		int err_code = sqlite3_step(statement);
		sqlite3_finalize(statement);

		if (SQLITE_DONE != err_code)
		{
			auto err = sqlite3_errstr(err_code);
			throw std::exception(err);
		}
	}

	sqlite3_close_v2(sqlite);*/
}

int OrderRepository::GetMaxRemoteId()
{
	int max_id = 0;
	sqlite::Connection db;
	db.Open(DATABASE_NAME);
	sqlite::Cursor crs(db, "select max(remote_id) from orders;");
	
	if (crs.Next())
	{
		max_id = crs.GetInt(0);
	}
	
	return max_id;
}

bool OrderRepository::exists(Order order)
{
	bool existing = false;
	/*
	std::stringstream strm;
	strm << "select id from orders where remote_id = " << order.GetRemoteId() << " LIMIT 1";
	
	sqlite3_open_v2(DATABASE_NAME, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(sqlite, strm.str().c_str(), strm.str().size(), &statement, nullptr) == SQLITE_OK)
	{
		if (SQLITE_ROW == sqlite3_step(statement))
		{
			existing = true;
		}
	}
	sqlite3_finalize(statement);
	sqlite3_close_v2(sqlite);*/

	return existing;
}

Order OrderRepository::doMakeOrder(sqlite::Cursor & crs)
{
	Order order;
	auto id = crs.GetInt(0);
	order.SetId(id);

	order.SetCustomerCode(Utils::s2ws(crs.GetText(1)));
	order.SetCustomerName(Utils::s2ws(crs.GetText(2)));
	order.SetCreationDate(Utils::s2ws(crs.GetText(3)));
	order.SetRemoteId(crs.GetInt(4));
	order.IsRead(crs.GetInt(5) == 1);
	
	return order;
}



/****************************
 *
 *   OrderItemRepository
 *
 ****************************/

OrderItemRepository::OrderItemRepository()
{
	/*sqlite::Connection db;
	db.Open(DATABASE_NAME);
	try
	{
		db.Execute("create table if not exists order_items (id integer primary key, order_id integer, code text not null, name text not null, qty integer, notes text not null, remote_id integer);");
	}
	catch (sqlite::SqliteException& exc)
	{
		throw std::exception(exc.what());
	}*/
}

OrderItemRepository::~OrderItemRepository()
{
}

void OrderItemRepository::Add(sqlite::Transaction & trn, OrderItem & orderItem, int orderId)
{
	std::string code, name, notes;
	code = Utils::to_string(orderItem.GetCode());
	name = Utils::to_string(orderItem.GetName());
	notes = Utils::to_string(orderItem.GetNotes());

	std::wostringstream insert;
	insert << "insert into order_items (order_id, code, name, qty, notes, remote_id) values(" <<
		orderId << ", " << Utils::ForSql(code) << ", " << Utils::ForSql(name) << ", " << orderItem.GetQty() <<
		", " << Utils::ForSql(notes) << ", " << orderItem.GetRemoteId() << ");";

	int rows_affected = trn.Execute(Utils::to_string(insert.str()).c_str());
	orderItem.SetId(trn.GetLastInsertRowid());
}

void OrderItemRepository::AddAll(sqlite::Transaction & trn, std::vector<OrderItem>& orderItems, int orderId)
{
	for (auto & item : orderItems)
	{
		Add(trn, item, orderId);
	}
	OutputDebugString(L"\t entro in OrderItemRepository::AddAll()\n");
}

std::vector<OrderItem> OrderItemRepository::GetAll(int orderId)
{
	auto orderItems = std::vector<OrderItem>();

	std::stringstream select;
	select << "select id, code, name, qty, notes, remote_id, order_id from order_items where order_id = " << orderId;

	sqlite::Connection db;
	db.Open(DATABASE_NAME);
	OutputDebugString(L"\t da OrderItemRepository::GetAll()\n");

	sqlite::Cursor crs(db, select.str().c_str());

	while (crs.Next())
	{
		orderItems.push_back(doMakeOrderItem(crs));
	}

	return orderItems;
}

OrderItem OrderItemRepository::doMakeOrderItem(sqlite::Cursor & crs)
{
	OrderItem orderItem;

	try
	{
		orderItem.SetId(crs.GetInt(0));
		orderItem.SetCode(Utils::s2ws(crs.GetText(1)));
		orderItem.SetName(Utils::s2ws(crs.GetText(2)));
		orderItem.SetQty(crs.GetInt(3));
		orderItem.SetNotes(Utils::s2ws(crs.GetText(4)));
		orderItem.SetRemoteId(crs.GetInt(5));
		orderItem.SetOrderId(crs.GetInt(6));
	}
	catch (std::exception&)
	{ }

	return orderItem;
}
