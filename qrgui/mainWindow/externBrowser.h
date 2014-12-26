#pragma once

#ifdef Q_WS_WIN
#include "mainWindow/windowsExternBrowser.h"
#endif

#ifdef Q_OS_LINUX
#include "mainWindow/linuxExternBrowser.h"
#endif

#ifdef Q_OS_MAC
#pragma message ("Mac OS is not needed, use Windows or Linux instead")
#include "mainWindow/macExternBrowser.h"
#endif
