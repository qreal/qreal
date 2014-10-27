#pragma once

#include <QtCore/qglobal.h>

#ifndef QRGUI_EDITOR_EXPORT
#  if defined(QRGUI_EDITOR_LIBRARY)
#    define QRGUI_EDITOR_EXPORT Q_DECL_EXPORT
#  else
#    define QRGUI_EDITOR_EXPORT Q_DECL_IMPORT
#  endif
#endif
