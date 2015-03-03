#include "windowsExternBrowser.h"

#include <windows.h>

ExternBrowser::ExternBrowser()
{
}

bool ExternBrowser::openPage(const QString &url)
{
	std::wstring wstr = url.toStdWString();// mb scheme()
	const wchar_t * temp = wstr.c_str();
	return int(ShellExecuteW(nullptr, nullptr, temp, nullptr, nullptr, SW_SHOWNORMAL)) > 32;  // zomg
}
