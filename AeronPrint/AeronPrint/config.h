#pragma once
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/fileconf.h>
#include <string>

class Config : public wxFileConfig
{
public:
	Config();
	~Config();
};

