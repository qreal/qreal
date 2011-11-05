#include "devRecordGenerator.h"

#include <QtCore/QFile>

#include <QtCore/QDebug>

using namespace ubiq::generator;

DevRecordGenerator::DevRecordGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
		: mTemplateDirPath(templateDirPath)
		, mOutputDirPath(outputDirPath)
		, mApi(logicalModel.logicalRepoApi())
		, mErrorReporter(errorReporter)
{
}

DevRecordGenerator::~DevRecordGenerator()
{
}

bool DevRecordGenerator::changeDir(const QString &path)
{
	if (!mDirectory.exists(path)) {
		qDebug() << "cannot find directory " << path;
		return false;
	}
	mDirectory.cd(path);
	return true;
}

bool DevRecordGenerator::loadTemplateFromFile(QString const &templateFileName, QString &loadedTemplate)
{
	if (!changeDir(mTemplateDirPath))
		return false;

	QString fileName = mDirectory.absoluteFilePath(templateFileName);
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "cannot load file \"" << fileName << "\"";
		return false;
	}
	QTextStream in(&file);
	loadedTemplate = in.readAll();

	file.close();
	mDirectory.cdUp();

	return true;
}

void DevRecordGenerator::generate()
{
	loadTemplateFromFile("DevRecord.cs", mFileTemplate);
	qDebug() << mFileTemplate;

}
