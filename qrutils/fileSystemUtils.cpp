/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "fileSystemUtils.h"

#include <QtCore/QFile>

#if defined(Q_OS_WIN)
#include <windows.h>
#endif

using namespace qReal;

bool FileSystemUtils::makeHidden(const QString &filePath)
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

void FileSystemUtils::resetAttributes(const QString &filePath)
{
#if defined(Q_OS_WIN)
	wchar_t *arr = (wchar_t*)filePath.utf16();
	SetFileAttributes(arr, FILE_ATTRIBUTE_NORMAL);
#else
	Q_UNUSED(filePath)
#endif
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
