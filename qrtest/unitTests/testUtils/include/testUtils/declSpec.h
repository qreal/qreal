#include <QtCore/qglobal.h>

#ifndef TEST_UTILS_EXPORT
#  if defined(TEST_UTILS_LIBRARY)
#    define TEST_UTILS_EXPORT Q_DECL_EXPORT
#  else
#    define TEST_UTILS_EXPORT Q_DECL_IMPORT
#  endif
#endif
