#pragma once

#include <QtCore/qglobal.h>

#ifndef QRGUI_THIRDPARTY_EXPORT
#  if defined(QRGUI_THIRDPARTY_LIBRARY)
#    define QRGUI_THIRDPARTY_EXPORT Q_DECL_EXPORT
#  else
#    define QRGUI_THIRDPARTY_EXPORT Q_DECL_IMPORT
#  endif
#endif
