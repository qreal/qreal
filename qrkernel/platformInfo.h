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

private:
#if defined Q_OS_WIN32
	QString prettyWindowsVersion(QSysInfo::WinVersion version);
#endif
};

}
