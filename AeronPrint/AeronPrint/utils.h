#pragma once
#include <string>
#include <sstream>

class Utils
{
public:
	Utils();
	~Utils();

	static void ReplaceAll(std::string& str, const std::string& from, const std::string& to);
	static std::wstring ForSql(const std::string& str);
	static std::string to_string(std::wstring s);
	static std::string to_string(const unsigned char * s);
	static std::string to_string(const char * s);
	static std::wstring s2ws(const std::string& str);
};

