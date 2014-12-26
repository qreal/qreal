#pragma once

#include <QtCore/qglobal.h>

#ifndef QRGUI_MOUSE_GESTURES_EXPORT
#  if defined(QRGUI_MOUSE_GESTURES_LIBRARY)
#    define QRGUI_MOUSE_GESTURES_EXPORT Q_DECL_EXPORT
#  else
#    define QRGUI_MOUSE_GESTURES_EXPORT Q_DECL_IMPORT
#  endif
#endif
