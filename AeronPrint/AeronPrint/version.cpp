
#include "version.h"
#include "utils.h"

Version::Version()
{
	TCHAR fileName[4096];
	GetModuleFileName(NULL, fileName, 4096);
	//theApp.m_pszExeName is no good

	DWORD dwHandle, dwLen;
	UINT BufLen;
	LPTSTR lpData;
	LPBYTE lpBuffer = NULL;
	dwLen = GetFileVersionInfoSize(fileName, &dwHandle);

	lpData = (LPTSTR)malloc(dwLen);
	if (!GetFileVersionInfo(fileName, dwHandle, dwLen, lpData))
	{
		free(lpData);
	}

	if (VerQueryValue(lpData, L"\\", (VOID FAR* FAR*)&lpBuffer, (PUINT)&BufLen))
	{
		if (BufLen)
		{
			VS_FIXEDFILEINFO *pFileInfo = (VS_FIXEDFILEINFO *)lpBuffer;
			MajorVersion = static_cast<unsigned int>(HIWORD(pFileInfo->dwFileVersionMS));
			MinorVersion = static_cast<unsigned int>(LOWORD(pFileInfo->dwFileVersionMS));
			BuildNumber = static_cast<unsigned int>(HIWORD(pFileInfo->dwFileVersionLS));
			RevisionNumber = static_cast<unsigned int>(LOWORD(pFileInfo->dwFileVersionLS));
		}
	}
	free(lpData);
}

std::string Version::ToString()
{
	
	char vers[20]{ 0 };
	sprintf_s(vers, "%hu.%hu.%hu.%hu", MajorVersion, MinorVersion, BuildNumber, RevisionNumber);
	//m_strVersion.Format(_T("%s, Version:%hu.%hu.%hu.%hu"), theApp.m_pszAppName, MajorVersion, MinorVersion, BuildNumber, RevisionNumber)
	return vers;
}
