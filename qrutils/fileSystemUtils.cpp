#include "fileSystemUtils.h"
#include <windows.h>

using namespace qReal;

bool FileSystemUtils::makeHidden(QString const &filePath)
{
#if defined(Q_OS_WIN)
	return SetFileAttributesW(filePath.toStdWString().c_str(), FILE_ATTRIBUTE_HIDDEN);
#else
	return false;
#endif
}
