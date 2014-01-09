#pragma once

#include <QtCore/qglobal.h>

#ifndef ROBOTS_INTERPRETER_BASE_EXPORT
#  if defined(ROBOTS_INTERPRETER_BASE_LIBRARY)
#    define ROBOTS_INTERPRETER_BASE_EXPORT Q_DECL_EXPORT
#  else
#    define ROBOTS_INTERPRETER_BASE_EXPORT Q_DECL_IMPORT
#  endif
#endif
