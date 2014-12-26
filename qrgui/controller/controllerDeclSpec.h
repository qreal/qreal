#pragma once

#include <QtCore/qglobal.h>

#ifndef QRGUI_CONTROLLER_EXPORT
#  if defined(QRGUI_CONTROLLER_LIBRARY)
#    define QRGUI_CONTROLLER_EXPORT Q_DECL_EXPORT
#  else
#    define QRGUI_CONTROLLER_EXPORT Q_DECL_IMPORT
#  endif
#endif
