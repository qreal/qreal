#pragma once

#include "../../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

#include <QtCore/QDir>

namespace ubiq {
namespace generator {

class DevRecordGenerator
{
public:
	DevRecordGenerator(QString const &templateDirPath
			, QString const &outputDirPath
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);
	virtual ~DevRecordGenerator();

	void generate();

private:

	bool changeDir(const QString &path);
	bool loadTemplateFromFile(QString const &templateFileName, QString &loadedTemplate);

	QString const mTemplateDirPath;
	QString const mOutputDirPath;
	qrRepo::LogicalRepoApi const &mApi;
	qReal::ErrorReporterInterface &mErrorReporter;

	QDir mDirectory;
	QString mFileTemplate;
};

}
}
