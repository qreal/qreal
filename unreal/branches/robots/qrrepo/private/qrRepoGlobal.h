#pragma once

#include <QtCore/qglobal.h>

#if defined(QRREPO_LIBRARY)
#  define QRREPO_EXPORT Q_DECL_EXPORT
#else
#  define QRREPO_EXPORT Q_DECL_IMPORT
#endif
