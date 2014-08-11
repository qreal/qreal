#pragma once

#include <QtCore/QString>

#include "kernelDeclSpec.h"

namespace qReal {

/// Contains useful methods for getting information about the external environment.
class QRKERNEL_EXPORT PlatformInfo
{
public:
	/// Returns readable information about the OS and its version.
	static QString printableInfo();

	/// Returns OS name in brief format (for example "Windows" or "Linux" or "Mac").
	static QString os();

private:
#if defined Q_OS_WIN32
	static QString prettyWindowsVersion(QSysInfo::WinVersion version);
#endif
};

}
