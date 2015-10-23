#include "order_repository.h"

OrderRepository::OrderRepository()
{
	//sqlite.OpenConnection(DATABASE_NAME, DATABASE_DIR);

	std::stringstream create;

	create << "create table if not exists orders" <<
		"(id INTEGER PRIMARY KEY, customer_code TEXT NOT NULL, customer_name TEXT NOT NULL, creation_date TEXT NOT NULL, " <<
		"remote_id INTEGER NOT NULL UNIQUE, read INTEGER DEFAULT(0));";

	sqlite3_open_v2(DATABASE_NAME, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
	int errcode = sqlite3_exec(sqlite, create.str().c_str(), nullptr, nullptr, nullptr);

	if (SQLITE_OK != errcode)
	{
		throw std::exception(sqlite3_errstr(errcode));
	}

	sqlite3_close_v2(sqlite);
}

OrderRepository::~OrderRepository()
{
	sqlite3_close_v2(sqlite);
}

void OrderRepository::Add(Order & order)
{
	if (!exists(order))
	{
		sqlite3_stmt *statement;
		int err_code = 0;

		std::string insert = "insert into orders(customer_code, customer_name, creation_date, remote_id, read) values(?, ?, ?, ?, ?);";

		sqlite3_open_v2(DATABASE_NAME, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);

		if (sqlite3_prepare_v2(sqlite, insert.c_str(), insert.size(), &statement, nullptr) == SQLITE_OK)
		{
			auto customerCode = to_string(order.GetCustomerCode());
			auto customerName = to_string(order.GetCustomerName());
			auto creationDate = to_string(order.GetCreationDate());
			auto remoteId = order.GetRemoteId();

			sqlite3_bind_text(statement, 1, customerCode.c_str(), customerCode.size(), SQLITE_STATIC);
			sqlite3_bind_text(statement, 2, customerName.c_str(), customerName.size(), SQLITE_STATIC);
			sqlite3_bind_text(statement, 3, creationDate.c_str(), creationDate.size(), SQLITE_STATIC);
			sqlite3_bind_int(statement, 4, remoteId);
			sqlite3_bind_int(statement, 5, order.IsRead());

			err_code = sqlite3_step(statement);
			sqlite3_finalize(statement);

			if (SQLITE_DONE != err_code)
			{
				auto err = sqlite3_errstr(err_code);
				throw std::exception(err);
			}

			order.SetId(sqlite3_last_insert_rowid(sqlite));
		}
		else
		{
			auto err = sqlite3_errstr(sqlite3_errcode(sqlite));
			throw std::exception(err);
		}

		sqlite3_close_v2(sqlite);
	}
}

Order OrderRepository::Get(int id)
{
	Order order;
	
	std::stringstream strm;
	strm << "select id, customer_code, customer_name, creation_date, remote_id, read from orders where id = " << id << " LIMIT 1";

	sqlite3_open_v2(DATABASE_NAME, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2(sqlite, strm.str().c_str(), strm.str().size(), &statement, nullptr) == SQLITE_OK)
	{
		if (SQLITE_DONE != sqlite3_step(statement))
		{
			order = build(statement);
		}
	}
	sqlite3_finalize(statement);
	sqlite3_close_v2(sqlite);
	return order;
}

std::vector<Order> OrderRepository::GetAll()
{
	std::vector<Order> orders;
	sqlite3_open_v2(DATABASE_NAME, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);

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
	sqlite3_close_v2(sqlite);
	return orders;
}

void OrderRepository::SaveAll(std::vector<Order> & orders)
{
	for (auto& order : orders)
	{
		Add(order);
	}
}

void OrderRepository::Remove(int id)
{
}

void OrderRepository::Update(Order order)
{
	sqlite3_open_v2(DATABASE_NAME, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
	
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

	sqlite3_close_v2(sqlite);
}

int OrderRepository::GetMaxRemoteId()
{
	int max_id = 0;
	sqlite3_open_v2(DATABASE_NAME, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);

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
	sqlite3_close_v2(sqlite);
	return max_id;
}

bool OrderRepository::exists(Order order)
{
	bool existing = false;
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
	sqlite3_close_v2(sqlite);

	return existing;
}

Order OrderRepository::build(sqlite3_stmt * statement)
{
	Order order;
	auto id = sqlite3_column_int(statement, 0);
	order.SetId(id);
	auto cc = sqlite3_column_text(statement, 1);
	order.SetCustomerCode(s2ws(to_string(cc)));
	order.SetCustomerName(s2ws(to_string(sqlite3_column_text(statement, 2))));
	order.SetCreationDate(s2ws(to_string(sqlite3_column_text(statement, 3))));
	order.SetRemoteId(sqlite3_column_int(statement, 4));
	order.IsRead(sqlite3_column_int(statement, 5) == 1);

	return order;
}

std::string OrderRepository::to_string(std::wstring s)
{
	std::string strValue;
	strValue.assign(s.begin(), s.end());
	return strValue;
}

std::string OrderRepository::to_string(const unsigned char * s)
{
	return reinterpret_cast<const char*>(s);
}

std::wstring OrderRepository::s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
