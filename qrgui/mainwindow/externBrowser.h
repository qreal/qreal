#ifdef Q_WS_WIN
#include "windowsExternBrowser.h"
#endif

#ifdef Q_OS_LINUX
#include "linuxExternBrowser.h"
#endif

#ifdef Q_OS_MAC
#pragma message ("Mac OS is not needed, use Windows or Linux instead")
#include "macExternBrowser.h"
#endif
