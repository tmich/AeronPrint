#pragma once
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <string>

const std::string APP_NAME = "AeronPrint";
const std::string VENDOR_NAME = "AEG2000";
const std::string CONFIG_LOCALNAME = "config.ini";
//const std::string CONFIG_FILE_NAME = CONFIG_PATH + wxString(wxFileName::GetPathSeparator()).ToStdString() + CONFIG_LOCALNAME;

class Config : public wxFileConfig
{
public:
	static std::string CONFIG_PATH;
	Config();
	~Config();
};

