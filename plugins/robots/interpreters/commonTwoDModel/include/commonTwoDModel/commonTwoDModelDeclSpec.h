#pragma once

#include <QtCore/qglobal.h>

#ifndef COMMON_TWO_D_MODEL_EXPORT
#  if defined(COMMON_TWO_D_MODEL_LIBRARY)
#    define COMMON_TWO_D_MODEL_EXPORT Q_DECL_EXPORT
#  else
#    define COMMON_TWO_D_MODEL_EXPORT Q_DECL_IMPORT
#  endif
#endif
