#include "config.h"
#include "wx\log.h"
#include <filesystem>

//namespace fs = std::experimental::filesystem;

std::string Config::CONFIG_PATH = "";

Config::Config() : wxFileConfig("AeronPrint", "AEG2000", "config.ini", "config.ini", wxCONFIG_USE_SUBDIR | wxCONFIG_USE_GLOBAL_FILE)
{
}


Config::~Config()
{
}

std::wstring Config::ApiPath()
{
	SetPath(wxT("/API"));
	auto path = Read("Path");
	return path.ToStdWstring();
}

long Config::PollingTime()
{
	SetPath(wxT("/API"));
	auto pollingTime = ReadLong("PollingTime", 60);
	return pollingTime;
}
