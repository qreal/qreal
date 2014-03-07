#pragma once

#ifdef Q_WS_WIN
#include "mainwindow/windowsExternBrowser.h"
#endif

#ifdef Q_OS_LINUX
#include "mainwindow/linuxExternBrowser.h"
#endif

#ifdef Q_OS_MAC
#pragma message ("Mac OS is not needed, use Windows or Linux instead")
#include "mainwindow/macExternBrowser.h"
#endif
