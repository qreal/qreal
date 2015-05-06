/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
