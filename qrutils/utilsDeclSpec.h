#pragma once

#include <QtCore/qglobal.h>

#if defined(QRUTILS_LIBRARY)
#  define QRUTILS_EXPORT Q_DECL_EXPORT
#else
#  define QRUTILS_EXPORT Q_DECL_IMPORT
#endif
