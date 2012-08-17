#pragma once

#include <QtCore/qglobal.h>

#ifdef UNITTEST
	#define QRKERNEL_EXPORT
#else
	#ifdef QRKERNEL_LIBRARY
		#define QRKERNEL_EXPORT Q_DECL_EXPORT
	#else
		#define QRKERNEL_EXPORT Q_DECL_IMPORT
	#endif
#endif
