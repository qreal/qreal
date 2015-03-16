#pragma once

#include <QtCore/qglobal.h>

#ifndef TWO_D_MODEL_EXPORT
#  if defined(TWO_D_MODEL_LIBRARY)
#    define TWO_D_MODEL_EXPORT Q_DECL_EXPORT
#  else
#    define TWO_D_MODEL_EXPORT Q_DECL_IMPORT
#  endif
#endif
