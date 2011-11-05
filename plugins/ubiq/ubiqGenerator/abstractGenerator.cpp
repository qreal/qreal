#include <QtCore/QFile>
#include <QtCore/QDebug>

#include "abstractGenerator.h"
#include "nameNormalizer.h"

using namespace ubiq::generator;
using namespace qReal;

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
		definition.replace("\\n", "\n");
		mTemplateUtils[name] = definition;
		line = in.readLine();
	}

	utilsFile.close();
	mDirectory.cdUp();

	return true;
}

void AbstractGenerator::saveOutputFile(QString const &fileName, QString const &content)
{
	QDir dir;

	if (!dir.exists(mOutputDirPath))
		dir.mkdir(mOutputDirPath);
	dir.cd(mOutputDirPath);

	QString const outputFileName = dir.absoluteFilePath(fileName);
	QFile file(outputFileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "cannot open \"" << outputFileName << "\"";
		return;
	}

	QTextStream out(&file);
	QString projectTemplate = content;
	out << content;
	file.close();
}

QString AbstractGenerator::generatePropertiesCode(Id const &element)
{
	QString properties;
	foreach (Id const property, mApi.children(element)) {
		if (!mApi.isLogicalElement(element) || element.element() != "Field")
			continue;

		// generate property code
		QString propertyTemplate = mTemplateUtils["@@Property@@"];
		QString name = mApi.name(property);
		propertyTemplate.replace("@Name@", NameNormalizer::normalize(name, false))
				.replace("@NameCaps@", NameNormalizer::normalize(name))
				.replace("@Type@", mApi.stringProperty(property, "type"));

		properties += propertyTemplate;
	}
	return properties;
}
