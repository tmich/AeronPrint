#pragma once
#include <wx/fileconf.h>
#include <string>

class Config : public wxFileConfig
{
public:
	Config();
	~Config();
};

