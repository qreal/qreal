#pragma once

#ifdef Q_OS_WIN
#include "windowsFantom.h"
#endif

#ifdef Q_OS_LINUX
#include "linuxFantom.h"
#endif

#ifdef Q_OS_MAC
#pragma message ("Mac OS is not needed, use Windows or Linux instead")
#include "macFantom.h"
#endif
