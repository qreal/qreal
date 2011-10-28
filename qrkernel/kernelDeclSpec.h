#pragma once

#include <QtCore/qglobal.h>

#if defined(QRKERNEL_LIBRARY)
#  define QRKERNEL_EXPORT Q_DECL_EXPORT
#else
#  define QRKERNEL_EXPORT Q_DECL_IMPORT
#endif
