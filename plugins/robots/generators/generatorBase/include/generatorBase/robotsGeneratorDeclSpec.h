#pragma once

#include <QtCore/qglobal.h>

#ifndef ROBOTS_GENERATOR_EXPORT
#  if defined(ROBOTS_GENERATOR_LIBRARY)
#    define ROBOTS_GENERATOR_EXPORT Q_DECL_EXPORT
#  else
#    define ROBOTS_GENERATOR_EXPORT Q_DECL_IMPORT
#  endif
#endif
