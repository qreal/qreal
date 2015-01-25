#pragma once

#include <QtCore/QString>

namespace qReal {

/// Provides some useful methods for getting information about external environment.
class PlatformInfo
{
public:
	/// Returns printable information about operating system which runs this application.
	static QString prettyOsVersion();
};

}
