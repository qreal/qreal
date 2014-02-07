#pragma once

#include <QtCore/qglobal.h>

#ifndef ROBOTS_UTILS_EXPORT
#  if defined(ROBOTS_UTILS_LIBRARY)
#    define ROBOTS_UTILS_EXPORT Q_DECL_EXPORT
#  else
#    define ROBOTS_UTILS_EXPORT Q_DECL_IMPORT
#  endif
#endif
