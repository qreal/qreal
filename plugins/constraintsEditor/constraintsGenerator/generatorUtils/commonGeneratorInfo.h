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

	void fillOneDiagram(const qReal::Id &diagram);

	QString hFilesString() const;

	QString cppFilesString() const;

	QString includeFilesPlugin() const;

	QString privateFieldsPlugin() const;

	QString ifForMainCheckPluginCpp() const;

private:
	QString replaceLanguageName(QString string
			, const qReal::Id &diagram
			, const int count);

	QString mHFiles;
	QString mCppFiles;
	QString mIncludeFilesPluginH;
	QString mPrivateFieldsPluginH;
	QString mIfForMainCheckPluginCPP;

	QMap<QString, QString> mTemplateUtils;
};

}
}
}
