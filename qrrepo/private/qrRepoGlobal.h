#pragma once

#include <QtCore/qglobal.h>

#ifdef UNITTEST
	#define QRREPO_EXPORT
#else
	#ifdef QRREPO_LIBRARY
		#define QRREPO_EXPORT Q_DECL_EXPORT
	#else
		#define QRREPO_EXPORT Q_DECL_IMPORT
	#endif
#endif
