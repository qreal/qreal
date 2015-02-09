#pragma once

#include <QtCore/QString>

#include "qrutils/utilsDeclSpec.h"

namespace qReal {

/// This class contains platform-dependent file system operations implementations
class QRUTILS_EXPORT FileSystemUtils
{
public:
	/// Tries hide specified file on windows and returns if operation was successfull.
	/// To make file hidden on Linux just make its name starting with '.'
	static bool makeHidden(const QString &filePath);
};

}
