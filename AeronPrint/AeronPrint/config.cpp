#include "config.h"
#include "wx\log.h"
#include <filesystem>

//namespace fs = std::experimental::filesystem;

Config::Config() : wxFileConfig("AeronPrint", "AEG2000", "config.ini", "config.ini", wxCONFIG_USE_SUBDIR | wxCONFIG_USE_GLOBAL_FILE)
{
	/*const std::string base_path = wxStandardPaths::Get().GetUserConfigDir().ToStdString();
	const std::string app_name{ "AeronPrint" };
	const std::string vendor_name{ "AEG2000" };
	const std::string sep = wxString(wxFileName::GetPathSeparator()).ToStdString();
	const std::string file_name{ "aeronprint.ini" };*/

	//const wxString ini_filename = base_path + sep + file_name;

	//try
	//{
	//	fs::path path(base_path + sep + vendor_name + sep + app_name);
	//	if (!fs::exists(path))
	//	{
	//		fs::create_directories(path);
	//	}
	//	
	//	wxLogDebug("ciao");

	//	//wxLogDebug(wxT("created inifile: %s"), file_name);
	//}
	//catch (std::exception&)
	//{
	//	wxLogDebug(wxT("Errore nella creazione del config file"));
	//}
}


Config::~Config()
{
}
