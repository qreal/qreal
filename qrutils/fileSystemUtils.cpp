#include <QtCore/QFile>

#include "fileSystemUtils.h"

using namespace utils;

bool FileSystemUtils::removeDir(QString const &dirPath, bool recursive)
{
	QDir dir(dirPath);
	if (!dir.exists()) {
		return true;
	}
	if (recursive && !clearDir(dir))
	{
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

bool FileSystemUtils::removeFile(const QString &filePath)
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
