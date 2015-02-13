#include "fileSystemUtils.h"

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
