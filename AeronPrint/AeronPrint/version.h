#pragma once
#define FILEVER        1,1,0,0
#define PRODUCTVER     1,1,0,0
#define STRFILEVER     "1, 1, 0, 0\0"
#define STRPRODUCTVER  "1, 1, 0, 0\0"

#include <string>
#include <Windows.h>

class Version
{
public:
	Version();
	std::string ToString();
private:
	unsigned int MajorVersion;
	unsigned int MinorVersion;
	unsigned int BuildNumber;
	unsigned int RevisionNumber;
};