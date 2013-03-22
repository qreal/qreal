#include "windows.h"
#include "windowsExternBrowser.h"

ExternBrowser::ExternBrowser()
{
}

bool ExternBrowser::openPage(QString const &url)
{
	std::wstring wstr = url.toStdWString();// mb scheme()
	const wchar_t * temp = wstr.c_str();
	return int(ShellExecuteW(NULL, NULL, temp, NULL, NULL, SW_SHOWNORMAL)) > 32;  // омг
}
