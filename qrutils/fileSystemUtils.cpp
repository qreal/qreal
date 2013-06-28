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
	return false;
#endif
}
