#pragma once

#include <QtCore/qglobal.h>

#ifndef QRGUI_SYSTEM_FACADE_EXPORT
#  if defined(QRGUI_SYSTEM_FACADE_LIBRARY)
#    define QRGUI_SYSTEM_FACADE_EXPORT Q_DECL_EXPORT
#  else
#    define QRGUI_SYSTEM_FACADE_EXPORT Q_DECL_IMPORT
#  endif
#endif
