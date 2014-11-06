#pragma once

#include <QtCore/qglobal.h>

#ifndef QRGUI_HOTKEY_MANAGER_EXPORT
#  if defined(QRGUI_HOTKEY_MANAGER_LIBRARY)
#    define QRGUI_HOTKEY_MANAGER_EXPORT Q_DECL_EXPORT
#  else
#    define QRGUI_HOTKEY_MANAGER_EXPORT Q_DECL_IMPORT
#  endif
#endif
