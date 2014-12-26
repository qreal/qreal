#pragma once

#include <QtCore/qglobal.h>

#ifndef QRGUI_MODELS_EXPORT
#  if defined(QRGUI_MODELS_LIBRARY)
#    define QRGUI_MODELS_EXPORT Q_DECL_EXPORT
#  else
#    define QRGUI_MODELS_EXPORT Q_DECL_IMPORT
#  endif
#endif
