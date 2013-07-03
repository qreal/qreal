#include <QtCore/QFile>

#include "fileSystemUtils.h"

#if defined(Q_OS_WIN)
#include <windows.h>
#endif

using namespace qReal;

bool FileSystemUtils::makeHidden(QString const &filePath)
{
#if defined(Q_OS_WIN)
	return SetFileAttributesW(filePath.toStdWString().c_str(), FILE_ATTRIBUTE_HIDDEN);
#else
	Q_UNUSED(filePath)
	return false;
#endif
}

bool FileSystemUtils::removeDir(QString const &dirPath, bool recursive)
{
	QDir dir(dirPath);
	if (!dir.exists()) {
		return true;
	}
	if (recursive && !clearDir(dir)) {
		return false;
	}
	return dir.rmdir(dirPath);
}

bool FileSystemUtils::clearDir(QString const &dirPath)
{
	QDir dir(dirPath);
	if (!dir.exists()) {
		return true;
	}
	return clearDir(dir);
}

bool FileSystemUtils::removeFile(QString const &filePath)
{
	QFile file(filePath);
	if (!file.exists()) {
		return true;
	}
	return file.remove();
}

bool FileSystemUtils::clearDir(QDir dir)
{
	foreach (QFileInfo const &fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot)) {
		if (fileInfo.isDir()) {
			if (!clearDir(QDir(fileInfo.filePath())) || !dir.rmdir(fileInfo.fileName())) {
				return false;
			}
		} else {
			if (!dir.remove(fileInfo.fileName())) {
				return false;
			}
		}
	}
	return true;
}
