#pragma once

#include "../../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

#include <QtCore/QDir>

namespace ubiq {
namespace generator {

QString const utilsFileName = "utils.template";
QString const utilsSeparator = "==========";

class AbstractGenerator
{
public:
	AbstractGenerator(QString const &templateDirPath
			, QString const &outputDirPath
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);
	virtual ~AbstractGenerator() {}

	virtual void generate() = 0;

protected:
	bool changeDir(QString const &path);
	bool loadTemplateFromFile(QString const &templateFileName, QString &loadedTemplate);
	bool loadTemplateUtils();
	void saveOutputFile(QString const &fileName, QString const &content);
	QString generatePropertiesCode(qReal::Id const &element);

	QString getDefaultValue(QString const &type);

	QString const mTemplateDirPath;
	QString const mOutputDirPath;
	qrRepo::LogicalRepoApi const &mApi;
	qReal::ErrorReporterInterface &mErrorReporter;

	QDir mDirectory;
	QMap<QString, QString> mTemplateUtils;
};

}
}
