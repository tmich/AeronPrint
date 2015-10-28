#include "order_repository.h"
#include "utils.h"

OrderRepository::OrderRepository()
{
	//sqlite.OpenConnection(DATABASE_NAME, DATABASE_DIR);

	std::stringstream create;

	create << "create table if not exists orders" <<
		"(id INTEGER PRIMARY KEY, customer_code TEXT NOT NULL, customer_name TEXT NOT NULL, creation_date TEXT NOT NULL, " <<
		"remote_id INTEGER NOT NULL UNIQUE, read INTEGER DEFAULT(0));";

	create << "create table if not exists order_items" <<
		"(id INTEGER PRIMARY KEY, order_id INTEGER, code TEXT NOT NULL, name TEXT NOT NULL, qty INTEGER, notes TEXT, " <<
		"remote_id INTEGER NOT NULL UNIQUE, FOREIGN KEY(order_id) REFERENCES orders(id));";
	
	MyDb db;
	//sqlite3_open_v2(DATABASE_NAME, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
	//int errcode = sqlite3_exec(sqlite, create.str().c_str(), nullptr, nullptr, nullptr);
	int errcode = db.Execute(create.str().c_str());

	if (db.GetLastError() != "")
	{
		throw std::exception(db.GetLastError().c_str());
	}

	//sqlite3_close_v2(sqlite);
}

OrderRepository::~OrderRepository()
{
	//sqlite3_close_v2(sqlite);
}

void OrderRepository::Add(Order & order)
{
	if (!exists(order))
	{
		//sqlite3_open_v2(DATABASE_NAME, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
		MyDb db;
		Add(db, order);
		//sqlite3_close_v2(sqlite);
	}
}

void OrderRepository::Add(SQLiteDB & db, Order & order)
{
	//sqlite3_stmt *statement;
	//int err_code = 0;
	
	/*if (sqlite3_prepare_v2(db, insert.c_str(), insert.size(), &statement, nullptr) == SQLITE_OK)
	{*/
		auto customerCode = Utils::to_string(order.GetCustomerCode());
		auto customerName = Utils::to_string(order.GetCustomerName());
		auto creationDate = Utils::to_string(order.GetCreationDate());
		auto remoteId = order.GetRemoteId();

		std::wostringstream insert;
		insert << "insert into orders(customer_code, customer_name, creation_date, remote_id, read) values(" <<
			Utils::ForSql(customerCode) << ", " << Utils::ForSql(customerName) << ", " << Utils::ForSql(creationDate) << 
			", " << remoteId << ", " << order.IsRead() << ");";

		/*sqlite3_bind_text(statement, 1, customerCode.c_str(), customerCode.size(), SQLITE_STATIC);
		sqlite3_bind_text(statement, 2, customerName.c_str(), customerName.size(), SQLITE_STATIC);
		sqlite3_bind_text(statement, 3, creationDate.c_str(), creationDate.size(), SQLITE_STATIC);
		sqlite3_bind_int(statement, 4, remoteId);
		sqlite3_bind_int(statement, 5, order.IsRead());*/

		//sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
		/*err_code = sqlite3_step(statement);
		sqlite3_finalize(statement);*/
		//sqlite3_exec(db, "COMMIT TRANSACTION", nullptr, nullptr, nullptr);

		int rows_affected = db.Execute(Utils::to_string(insert.str()).c_str());
		if (1 != rows_affected)
		{
			auto err = db.GetLastError();
			throw std::exception(err.c_str());
		}

		order.SetId(sqlite3_last_insert_rowid(db));

		for (auto & orderItem : order.Items)
		{
			addItem(db, orderItem, order.GetId());
		}
	//}
	/*else
	{
		auto err = sqlite3_errstr(sqlite3_errcode(sqlite));
		throw std::exception(err);
	}*/
}

Order OrderRepository::Get(int id)
{
	Order order;
	
	std::stringstream select;
	select << "select id, customer_code, customer_name, creation_date, remote_id, read from orders where id = " << id << " LIMIT 1";

	/*sqlite3_open_v2(DATABASE_NAME, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
	sqlite3_stmt *statement;*/

	MyDb db;
	auto res = db.ExecuteSelect(select.str().c_str());
	if (res != nullptr)
	{
		if (res->Next())
		{
			order = build(res);
		}
	}

	return order;
}

std::vector<Order> OrderRepository::GetAll()
{
	std::vector<Order> orders;
	/*sqlite3_open_v2(DATABASE_NAME, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);

	sqlite3_stmt *statement;
	std::string query = "select id, customer_code, customer_name, creation_date, remote_id, read from orders;";
	if (sqlite3_prepare_v2(sqlite, query.c_str(), query.size(), &statement, nullptr) == SQLITE_OK)
	{
		while (SQLITE_ROW == sqlite3_step(statement))
		{
			Order order = build(statement);
			orders.push_back(order);
		}
	}
	sqlite3_finalize(statement);
	sqlite3_close_v2(sqlite);*/
	return orders;
}

void OrderRepository::AddAll(std::vector<Order> & orders)
{
	//sqlite3_open_v2(DATABASE_NAME, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
	//sqlite3_exec(sqlite, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
	SQLiteDB db;
	db.OpenConnection(DATABASE_NAME, DATABASE_DIR);
	db.BeginTransaction();

	try
	{
		for (auto& order : orders)
		{
			Add(db, order);
		}

		//sqlite3_exec(sqlite, "COMMIT TRANSACTION", nullptr, nullptr, nullptr);
		db.CommitTransaction();
	}
	catch (const std::exception&)
	{
		//sqlite3_exec(sqlite, "ROLLBACK TRANSACTION", nullptr, nullptr, nullptr);
		db.RollbackTransaction();
		throw;
	}
}

void OrderRepository::Remove(int id)
{
}

void OrderRepository::Update(Order order)
{
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
	/*sqlite3_open_v2(DATABASE_NAME, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);

	sqlite3_stmt *statement;
	std::string query = "select max(remote_id) from orders;";
	if (sqlite3_prepare_v2(sqlite, query.c_str(), query.size(), &statement, nullptr) == SQLITE_OK)
	{
		if (SQLITE_ROW == sqlite3_step(statement))
		{
			max_id = sqlite3_column_int(statement, 0);
		}
	}
	sqlite3_finalize(statement);
	sqlite3_close_v2(sqlite);*/
	return max_id;
}

bool OrderRepository::exists(Order order)
{
	bool existing = false;
	/*std::stringstream strm;
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

Order OrderRepository::build(IResult * res)
{
	Order order;
	auto id = atoi(res->ColumnData(0));
	order.SetId(id);

	auto cc = res->ColumnData(1);
	order.SetCustomerCode(Utils::s2ws(std::string(cc)));
	order.SetCustomerName(Utils::s2ws(std::string(res->ColumnData(2))));
	order.SetCreationDate(Utils::s2ws(std::string(res->ColumnData(3))));
	order.SetRemoteId(atoi(res->ColumnData(4)));
	order.IsRead(atoi(res->ColumnData(5)) == 1);

	SQLiteDB db;
	db.OpenConnection(DATABASE_NAME, DATABASE_DIR);
	std::ostringstream sql;
	sql << "select id, code, name, qty, notes, remote_id from order_items where order_id = " << id;
	auto res2 = db.ExecuteSelect(sql.str().c_str());

	if (res2 != nullptr)
	{
		while (res2->Next())
		{
			OrderItem itm;
			itm.SetId(atoi(res2->ColumnData(0)));
			itm.SetCode(Utils::s2ws(std::string(res2->ColumnData(1))));
			itm.SetName(Utils::s2ws(std::string(res2->ColumnData(2))));
			itm.SetQty(atoi(res2->ColumnData(3)));
			itm.SetNotes(Utils::s2ws(std::string(res2->ColumnData(4))));
			itm.SetRemoteId(atoi(res2->ColumnData(5)));
			order.Items.push_back(itm);
		}
	}

	return order;
}



void OrderRepository::addItem(SQLiteDB & db, OrderItem & item, int orderId)
{
	std::string code, name, notes;
	code = Utils::to_string(item.GetCode());
	name = Utils::to_string(item.GetName());
	notes = Utils::to_string(item.GetNotes());

	std::wostringstream insert;
	insert << "insert into order_items (order_id, code, name, qty, notes, remote_id) values(" <<
		orderId << ", " << Utils::ForSql(code) << ", " << Utils::ForSql(name) << ", " << item.GetQty() << 
		", " << Utils::ForSql(notes) << ", " << item.GetRemoteId() << ");";
		
	int rows_affected = db.Execute(Utils::to_string(insert.str()).c_str());

	if (1 != rows_affected)
	{
		auto err = db.GetLastError();
		throw std::exception(err.c_str());
	}

	item.SetId(sqlite3_last_insert_rowid(db));
}
