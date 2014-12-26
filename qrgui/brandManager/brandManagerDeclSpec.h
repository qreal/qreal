#pragma once

#include <QtCore/qglobal.h>

#ifndef QRGUI_BRAND_MANAGER_EXPORT
#  if defined(QRGUI_BRAND_MANAGER_LIBRARY)
#    define QRGUI_BRAND_MANAGER_EXPORT Q_DECL_EXPORT
#  else
#    define QRGUI_BRAND_MANAGER_EXPORT Q_DECL_IMPORT
#  endif
#endif
