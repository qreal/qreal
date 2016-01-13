/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
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

#include <QtCore/QDebug>
#include <QtCore/QDir>

#if defined(Q_OS_WIN)
#include <windows.h>
#endif

using namespace utils;

bool FileSystemUtils::makeHidden(const QString &filePath)
{
#if defined(Q_OS_WIN)
	return SetFileAttributesW(filePath.toStdWString().c_str(), FILE_ATTRIBUTE_HIDDEN);
#else
	Q_UNUSED(filePath)
	return false;
#endif
}

void FileSystemUtils::clearDir(const QString &path)
{
	if (path.isEmpty()) {
		return;
	}

	QDir dir(path);
	if (!dir.exists()) {
		return;
	}

	foreach (const QFileInfo &fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
		if (fileInfo.isDir()) {
			clearDir(fileInfo.filePath());
		} else {
			dir.remove(fileInfo.fileName());
		}
	}

	dir.rmdir(path);
}

bool FileSystemUtils::setCreationDateToNow(const QString &path)
{
#if defined(Q_OS_WIN)
	// Getting file handle
	const LPCTSTR fileName = sizeof(TCHAR) == 1
			? (LPCTSTR)path.toLocal8Bit().constData()
			: (LPCTSTR)path.utf16();
	const HANDLE handle = CreateFile(fileName, FILE_WRITE_ATTRIBUTES
			, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (handle == INVALID_HANDLE_VALUE) {
		qWarning() << "FileSystemUtils::setCreationDateToNow: Could not open" << path << "for writing";
		return false;
	}

	FILETIME fileTime;
	SYSTEMTIME systemTime;

	GetSystemTime(&systemTime);
	SystemTimeToFileTime(&systemTime, &fileTime);
	bool result = SetFileTime(handle, &fileTime, static_cast<LPFILETIME>(nullptr), static_cast<LPFILETIME>(nullptr));
	result &= CloseHandle(handle);
	return result;
#else
	Q_UNUSED(path)
	return false;
#endif
}
