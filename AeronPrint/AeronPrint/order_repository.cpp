#include "order_repository.h"

OrderRepository::OrderRepository()
{
	SQLiteDB sqlite;
	uint32_t result = sqlite.Execute("create table if not exists orders(id integer unique, customer_code text, customer_name text, creation_date text);");
}

OrderRepository::~OrderRepository()
{
}

void OrderRepository::Add(Order order)
{
	/*std::string s = "replace into orders(id, customer_code, customer_name, creation_date) values(?, ?, ?, ?);";
	sqlite3_stmt *statement;
	char *query = &s[0];
	{
		DbHelper db;
		if (sqlite3_prepare_v2(db, query, -1, &statement, 0) == SQLITE_OK)
		{
			sqlite3_bind_int(statement, 0, order.GetId());
			sqlite3_bind_text(statement, 1, to_string(order.GetCustomerCode()).c_str(), order.GetCustomerCode().size(), SQLITE_STATIC);
			sqlite3_bind_text(statement, 2, to_string(order.GetCustomerName()).c_str(), order.GetCustomerName().size(), SQLITE_STATIC);
			sqlite3_bind_text(statement, 3, to_string(order.GetCreationDate()).c_str(), order.GetCreationDate().size(), SQLITE_STATIC);

			int res = sqlite3_step(statement);
			sqlite3_finalize(statement);
		}
	}*/
}

Order OrderRepository::Get(int id)
{
	//sqlite3_stmt *statement;
	Order order;
	
	//std::stringstream strm;

	//strm << "select id, customer_code, customer_name, creation_date from orders where id = " << id << " LIMIT 1";
	//std::string s = strm.str();
	//char *str = &s[0];

	//char *query = str;
	//{
	//	DbHelper db;
	//	if (sqlite3_prepare(db, query, -1, &statement, 0) == SQLITE_OK)
	//	{
	//		int ctotal = sqlite3_column_count(statement);
	//		int res = 0;

	//		/*while (1)
	//		{*/
	//			res = sqlite3_step(statement);

	//			if (res == SQLITE_ROW)
	//			{
	//				//for (int i = 0; i < ctotal; i++)
	//				//{

	//				int id = sqlite3_column_int(statement, 0);						// col 1: ID
	//				std::wstring cc = (wchar_t*)sqlite3_column_text(statement, 1);	// col 2: customer code
	//				std::wstring cn = (wchar_t*)sqlite3_column_text(statement, 2);	// col 3: customer name
	//				std::wstring cd = (wchar_t*)sqlite3_column_text(statement, 3);	// col 4: creation date

	//				order.SetId(id);
	//				order.SetCustomerCode(cc);
	//				order.SetCustomerName(cn);
	//				order.SetCreationDate(cd);
	//				//}
	//			}

	//			/*if (res == SQLITE_DONE || res == SQLITE_ERROR)
	//			{
	//				break;
	//			}*/
	//		//}
	//	}
	//}

	std::stringstream strm;
	strm << "select id, customer_code, customer_name, creation_date, remote_id from orders where id = " << id << " LIMIT 1";

	SQLiteDB sqlite;
	IResult *res = sqlite.ExecuteSelect(strm.str().c_str());

	if (res->Next())
	{
		order.SetId(atoi(res->ColumnData(0)));
		order.SetCustomerCode(s2ws(res->ColumnData(1)));
		order.SetCustomerName(s2ws(res->ColumnData(2)));
		order.SetCreationDate(s2ws(res->ColumnData(3)));
		order.SetRemoteId(atoi(res->ColumnData(4)));
	}

	return order;
}

void OrderRepository::Remove(int id)
{
}

int OrderRepository::GetMaxRemoteId()
{
	int max_id = 0;
	SQLiteDB sqlite;
	IResult *res = sqlite.ExecuteSelect("select max(remote_id) from orders;");

	if (res->Next())
	{
		max_id = atoi(res->ColumnData(0));
	}

	return max_id;
}

std::string OrderRepository::to_string(std::wstring s)
{
	std::string strValue;
	strValue.assign(s.begin(), s.end());
	return strValue;
}

std::wstring OrderRepository::s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
