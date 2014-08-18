#pragma once

#include <QtCore/qglobal.h>

#ifndef QRTEXT_EXPORT
#  if defined(QRTEXT_LIBRARY)
#    define QRTEXT_EXPORT Q_DECL_EXPORT
#  else
#    define QRTEXT_EXPORT Q_DECL_IMPORT
#  endif
#endif
