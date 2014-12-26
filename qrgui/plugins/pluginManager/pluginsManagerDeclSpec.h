#pragma once

#include <QtCore/qglobal.h>

#ifndef QRGUI_PLUGINS_MANAGER_EXPORT
#  if defined(QRGUI_PLUGINS_MANAGER_LIBRARY)
#    define QRGUI_PLUGINS_MANAGER_EXPORT Q_DECL_EXPORT
#  else
#    define QRGUI_PLUGINS_MANAGER_EXPORT Q_DECL_IMPORT
#  endif
#endif
