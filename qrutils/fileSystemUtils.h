#pragma once

#include <QtCore/QString>
#include <QtCore/QDir>

#include "utilsDeclSpec.h"

namespace qReal {

/// This class contains platform-dependent file system operations implementations
class QRUTILS_EXPORT FileSystemUtils
{
public:
	/// Tries hide specified file on windows and returns if operation was successfull.
	/// To make file hidden on Linux just make its name starting with '.'
	static bool makeHidden(QString const &filePath);

	/// Removes specified directory from disk. Returns if operation succeeded
	/// @param dirPath Path to removing directory
	/// @param recursive If true, the directory will be recursively cleared.
	/// If false and directory was nonempty then operation fails
	static bool removeDir(QString const &dirPath, bool const recursive = true);

	/// Clears specified directory
	/// @param dirPath Path to clearing directory
	static bool clearDir(QString const &dirPath);

	/// Removes specified file. Returns if operation succeeded
	static bool removeFile(QString const &filePath);

	static void resetAttributes(QString const &filePath);

private:
	static bool clearDir(QDir dir);
};

}
