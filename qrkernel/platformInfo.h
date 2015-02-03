#pragma once

#include <qrkernel/kernelDeclSpec.h>

namespace qReal {

/// Provides some useful methods for getting information about external environment.
class QRKERNEL_EXPORT PlatformInfo
{
public:
	/// Returns printable information about operating system which runs this application.
	static QString prettyOsVersion();
};

}
