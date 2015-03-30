#pragma once

#include <QtCore/qglobal.h>

#ifndef ROBOTS_EV3_GENERATOR_BASE_EXPORT
#  if defined(ROBOTS_EV3_GENERATOR_BASE_LIBRARY)
#    define ROBOTS_EV3_GENERATOR_BASE_EXPORT Q_DECL_EXPORT
#  else
#    define ROBOTS_EV3_GENERATOR_BASE_EXPORT Q_DECL_IMPORT
#  endif
#endif
