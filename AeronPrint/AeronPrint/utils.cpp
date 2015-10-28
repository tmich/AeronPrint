#include "utils.h"
#include <Windows.h>

Utils::Utils()
{
}


Utils::~Utils()
{
}

void Utils::ReplaceAll(std::string & str, const std::string & from, const std::string & to)
{
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

std::wstring Utils::ForSql(const std::string & str)
{
	std::string newstring(str);
	ReplaceAll(newstring, "'", "''");
	std::wostringstream sstr;
	sstr << std::wstring(L"'") << s2ws(newstring) << std::wstring(L"'");
	return sstr.str();
}

std::string Utils::to_string(std::wstring s)
{
	std::string strValue;
	strValue.assign(s.begin(), s.end());
	return strValue;
}

std::string Utils::to_string(const unsigned char * s)
{
	return reinterpret_cast<const char*>(s);
}

std::string Utils::to_string(const char * s)
{
	return reinterpret_cast<const char*>(s);
}

std::wstring Utils::s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}