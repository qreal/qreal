#pragma once

#include <QtCore/qglobal.h>

#ifdef UNITTEST
	#define QRUTILS_EXPORT
#else
	#ifdef QRUTILS_LIBRARY
		#define QRUTILS_EXPORT Q_DECL_EXPORT
	#else
		#define QRUTILS_EXPORT Q_DECL_IMPORT
	#endif
#endif
