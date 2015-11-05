#include "MyApp.h"
#include "orders_view.h"
#include "sqlite_db.h"
#include "config.h"
#include <filesystem>

namespace fs = std::experimental::filesystem;

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	// call the base class initialization method, currently it only parses a
	// few common command-line options but it could be do more in the future
	if (!wxApp::OnInit())
		return false;

	SetAppName("AeronPrint");
	SetVendorName("AEG2000");

	// open config file
	Config cfg;
	if (!cfg.HasGroup(wxT("API")))
	{
		cfg.SetPath(wxT("/API"));
		cfg.Write(wxT("Path"), "http://localhost:5000/api/v1.0/");
	}
	
	if (!cfg.HasGroup(wxT("TEST")))
	{
		cfg.SetPath(wxT("/TEST"));
		cfg.Write(wxT("Dummy"), 18);
	}

	cfg.Flush();
	//cfg.Write(wxT("/dummy_integer"), 100);
	//cfg.Flush();
	
	// check database
	sqlite::Connection db;
	db.Open(DATABASE_NAME);
	try
	{
		db.Execute("create table if not exists orders (id integer primary key, customer_code text not null, customer_name text not null, creation_date text not null, remote_id integer, read integer);");
		db.Execute("create table if not exists order_items (id integer primary key, order_id integer, code text not null, name text not null, qty integer, notes text not null, remote_id integer);");
	}
	catch (sqlite::SqliteException& exc)
	{
		throw std::exception(exc.what());
		return false;
	}

	// create controller
	ordersController = new OrdersController(new OrdersView("AEG 2000"));

	return true;
}

int MyApp::OnExit()
{
	delete ordersController;
	return wxApp::OnExit();
}

MyApp::MyApp()
{

}
