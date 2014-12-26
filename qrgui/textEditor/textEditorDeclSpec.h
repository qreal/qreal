#pragma once

#include <QtCore/qglobal.h>

#ifndef QRGUI_TEXT_EDITOR_EXPORT
#  if defined(QRGUI_TEXT_EDITOR_LIBRARY)
#    define QRGUI_TEXT_EDITOR_EXPORT Q_DECL_EXPORT
#  else
#    define QRGUI_TEXT_EDITOR_EXPORT Q_DECL_IMPORT
#  endif
#endif
