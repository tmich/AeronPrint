#include "MyApp.h"
#include "orders_view.h"
#include "sqlite_db.h"
#include "config.h"
#include <filesystem>
#include <wx/stdpaths.h>
#include <wx/filename.h>

namespace fs = std::experimental::filesystem;

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	// call the base class initialization method, currently it only parses a
	// few common command-line options but it could be do more in the future
	if (!wxApp::OnInit())
		return false;

	// Check config directories
	const std::string CONFIG_PATH = (wxStandardPaths::Get().GetUserConfigDir() + wxFileName::GetPathSeparator()
		+ APP_NAME).ToStdString();
	fs::path path(CONFIG_PATH);
	if (!fs::exists(path))
	{
		fs::create_directories(path);
	}
	
	// open config file
	Config cfg;
	cfg.CONFIG_PATH = CONFIG_PATH;

	/* API ADDRESS */
	if (!cfg.HasGroup(wxT("API")))
	{
		cfg.SetPath(wxT("/API"));
		cfg.Write(wxT("Path"), "http://80.211.2.91/api/v1.0/");
	}

	/* POLLING TIME */
	cfg.SetPath(wxT("/API"));
	auto PollingTime = cfg.ReadLong(wxT("PollingTime"), 0);

	if (PollingTime < 60)
	{
		cfg.Write(wxT("PollingTime"), 60);
	}
	
	cfg.Flush();

	// check database
	sqlite::Connection::DATABASE_PATH = CONFIG_PATH;

	sqlite::Connection db;
	db.Open(DATABASE_NAME);
	try
	{
		db.Execute("create table if not exists orders (id integer primary key, customer_code text not null, customer_name text not null, creation_date text not null, remote_id integer, read integer);");
		db.Execute("create table if not exists order_items (id integer primary key, order_id integer, code text not null, name text not null, qty integer, notes text not null, remote_id integer);");
	}
	catch (sqlite::SqliteException& exc)
	{
		wxMessageBox(exc.what());
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
