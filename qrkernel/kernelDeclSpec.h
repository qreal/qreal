#pragma once

#include <QtCore/qglobal.h>

#ifndef QRKERNEL_EXPORT
#  if defined(UNITTEST)
#    define QRKERNEL_EXPORT
#  else
#    if defined(QRKERNEL_LIBRARY)
#      define QRKERNEL_EXPORT Q_DECL_EXPORT
#    else
#      define QRKERNEL_EXPORT Q_DECL_IMPORT
#    endif
#  endif
#endif
