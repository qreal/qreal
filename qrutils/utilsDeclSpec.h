#pragma once

#include <QtCore/qglobal.h>

#ifndef QRUTILS_EXPORT
#  if defined(QRUTILS_LIBRARY)
#    define QRUTILS_EXPORT Q_DECL_EXPORT
#  else
#    define QRUTILS_EXPORT Q_DECL_IMPORT
#  endif
#endif
