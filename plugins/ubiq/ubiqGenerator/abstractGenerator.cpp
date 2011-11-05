#include <QtCore/QFile>
#include <QtCore/QDebug>

#include "abstractGenerator.h"

using namespace ubiq::generator;

AbstractGenerator::AbstractGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
		: mTemplateDirPath(templateDirPath)
		, mOutputDirPath(outputDirPath)
		, mApi(logicalModel.logicalRepoApi())
		, mErrorReporter(errorReporter)
{
	mTemplateUtils.clear();
}

bool AbstractGenerator::changeDir(const QString &path)
{
	if (!mDirectory.exists(path)) {
		qDebug() << "cannot find directory " << path;
		return false;
	}
	mDirectory.cd(path);
	return true;
}

bool AbstractGenerator::loadTemplateFromFile(QString const &templateFileName, QString &loadedTemplate)
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

bool AbstractGenerator::loadTemplateUtils()
{
	if (!changeDir(mTemplateDirPath))
		return false;

	mTemplateUtils.clear();

	QString fileName = mDirectory.absoluteFilePath(utilsFileName);
	QFile utilsFile(fileName);
	if (!utilsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "cannot load file \"" << fileName << "\"";
		return false;
	}
	QTextStream in(&utilsFile);
	QString line = in.readLine();
	while (!line.isNull()) {
		QString name = line.section("=", 0, 0);
		QString definition = line.section("=", 1);
		mTemplateUtils[name] = definition;
		line = in.readLine();
	}

	utilsFile.close();
	mDirectory.cdUp();

	return true;
}

