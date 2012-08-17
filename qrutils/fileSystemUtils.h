#pragma once

#include <QtCore/QDir>

#include "utilsDeclSpec.h"

namespace utils
{

class QRUTILS_EXPORT FileSystemUtils
{
public:
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

private:
	static bool clearDir(QDir dir);
};

}
