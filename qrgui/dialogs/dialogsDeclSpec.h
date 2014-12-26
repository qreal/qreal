#pragma once

#include <QtCore/qglobal.h>

#ifndef QRGUI_DIALOGS_EXPORT
#  if defined(QRGUI_DIALOGS_LIBRARY)
#    define QRGUI_DIALOGS_EXPORT Q_DECL_EXPORT
#  else
#    define QRGUI_DIALOGS_EXPORT Q_DECL_IMPORT
#  endif
#endif
