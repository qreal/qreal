#pragma once

#include <QtCore/QString>
#include <QtCore/QMap>

#include <qrkernel/ids.h>

namespace constraints {
namespace generator {
namespace generatorUtils {

class CommonGeneratorInfo
{
public:
	CommonGeneratorInfo(const QMap<QString, QString> &templateUtils);

private:
	QString replaceLanguageName(QString string
			, const qReal::Id &diagram
			, const int count);

	QString mHFiles;
	QString mCppFiles;
	QString mIncludeFilesPluginH;
	QString mPrivateFieldsPluginH;
	QString mIfForMainCheckPluginCPP;
};

}
}
}
