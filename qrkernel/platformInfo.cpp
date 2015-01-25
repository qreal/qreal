#include "platformInfo.h"

using namespace qReal;

#if defined Q_OS_WIN32
QString prettyWindowsVersion(QSysInfo::WinVersion version)
{
	// Adapted from https://qt.gitorious.org/qt/thiago-intels-qtbase/commit/5757c458157bcb11da40c43e98eeb7a539d20912
	switch (version)
	{
	case QSysInfo::WV_95:
		return "95";
	case QSysInfo::WV_98:
		return "98";
	case QSysInfo::WV_Me:
		return "ME";
	case QSysInfo::WV_NT:
		return "NT";
	case QSysInfo::WV_2000:
		return "2000";
	case QSysInfo::WV_XP:
		return "XP";
	case QSysInfo::WV_2003:
		return "2003";
	case QSysInfo::WV_VISTA:
		return "Vista";
	case QSysInfo::WV_WINDOWS7:
		return "7";
	case QSysInfo::WV_WINDOWS8:
		return "8";
	case QSysInfo::WV_WINDOWS8_1:
		return "8.1";
	case QSysInfo::WV_CE:
		return "CE";
	case QSysInfo::WV_CENET:
		return "CENET";
	case QSysInfo::WV_CE_5:
		return "CE5";
	case QSysInfo::WV_CE_6:
		return "CE6";
	default:
		return "";
	}
}
#endif

QString PlatformInfo::prettyOsVersion()
{
#if defined Q_OS_WIN32
	return QString("Windows ") + prettyWindowsVersion(QSysInfo().windowsVersion());
#elif defined Q_OS_LINUX
	return "Linux";
#elif defined Q_OS_MAC
	return QString("Mac ") + QSysInfo().macVersion();
#endif
}
