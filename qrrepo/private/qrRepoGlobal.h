#pragma once

#include <QtCore/qglobal.h>

#ifndef QRREPO_EXPORT
#  if defined(UNITTEST)
#    define QRREPO_EXPORT
#  else
#    if defined(QRREPO_LIBRARY)
#      define QRREPO_EXPORT Q_DECL_EXPORT
#    else
#      define QRREPO_EXPORT Q_DECL_IMPORT
#    endif
#  endif
#endif
