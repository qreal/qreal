#include "virtualKeyboard.h"

#include <QtCore/QSysInfo>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>

using namespace utils;

void VirtualKeyboard::show()
{
#ifdef Q_OS_WIN
	switch(QSysInfo::windowsVersion()) {
	case QSysInfo::WV_WINDOWS8:
		showWindows8Keyboard();
		break;
	default:
		break;
	}
#endif
}

void VirtualKeyboard::showWindows8Keyboard()
{
	const QString programFilesPath(qgetenv("PROGRAMFILES").replace("\\", "/"));
	const QString tabTipPath = QString("%1/Common Files/microsoft shared/ink/TabTip.exe").arg(programFilesPath);
	if (QFileInfo(tabTipPath).exists()) {
		QDesktopServices::openUrl(QUrl("file:///" + tabTipPath));
	} else {
		const QString windowsDirectoryPath(qgetenv("WINDIR").replace("\\", "/"));
		const QString oskPath = QString("%1/system32/osk.exe").arg(windowsDirectoryPath);
		if (QFileInfo(oskPath).exists()) {
			QDesktopServices::openUrl(QUrl("file:///" + oskPath));
		}
	}
}
