#pragma once

#include <QtCore/qglobal.h>

#ifndef ROBOTS_KIT_BASE_EXPORT
#  if defined(ROBOTS_KIT_BASE_LIBRARY)
#    define ROBOTS_KIT_BASE_EXPORT Q_DECL_EXPORT
#  else
#    define ROBOTS_KIT_BASE_EXPORT Q_DECL_IMPORT
#  endif
#endif
