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

bool AbstractGenerator::loadUtilsFromFile()
{
	if (!changeDir(mTemplateDirPath))
		return false;

	QString fileName = mDirectory.absoluteFilePath(utilsFileName);
	QFile utilsFile(fileName);
	if (!utilsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "cannot load file \"" << fileName << "\"";
		return false;
	}
	QTextStream in(&utilsFile);

	QString line = in.readLine();
	do {
		// first line is name, everything else before the separator is template body

		QString name = line;
		QString body;
		line = in.readLine();
		while (!line.contains(utilsSeparator) && !line.isNull()) {
			body += (line + '\n');
			line = in.readLine();
		}
		mTemplateUtils[name] = body;
		line = in.readLine();
	} while (!line.isNull());

	utilsFile.close();
	mDirectory.cdUp();

	return true;
}

bool AbstractGenerator::loadUtilsFromDir()
{
	if (!changeDir(mTemplateDirPath + "/utils"))
		return false;

	QStringList files = mDirectory.entryList(QStringList());

	foreach (QString const fileName, files) {
		if (fileName == "." || fileName == "..")
			continue;

		// file name is template name, file contents is template body

		QString file = mDirectory.absoluteFilePath(fileName);
		QFile templateFile(file);

		if (!templateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			qDebug() << "cannot load file \"" << file << "\"";
			return false;
		}

		QTextStream in(&templateFile);

		QString line = in.readLine();
		QString body = line;
		while (!line.isNull()) {
			body += (line + '\n');
			line = in.readLine();
		}

		QString name = "@@" + fileName + "@@";
		if (!mTemplateUtils.contains(name))
			mTemplateUtils[name] = body;

		templateFile.close();
	}
	mDirectory.cdUp();
	mDirectory.cdUp();

	return true;
}


bool AbstractGenerator::loadUtilsTemplates()
{
	mTemplateUtils.clear();

	return loadUtilsFromFile() && loadUtilsFromDir();
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

QString AbstractGenerator::getDefaultValue(QString const &type)
{
	if (type == "short" || type == "int")
		return "0";

	return "new " + type + "()";
}

QString AbstractGenerator::generatePropertiesCode(Id const &element)
{
	QString properties;
	foreach (Id const property, mApi.children(element)) {
		if (!mApi.isLogicalElement(property) || property.element() != "Field")
			continue;

		// generate property code
		QString propertyTemplate = mTemplateUtils["@@Property@@"];
		QString name = mApi.name(property);
		propertyTemplate.replace("@@Name@@", NameNormalizer::normalize(name, false))
				.replace("@@NameCaps@@", NameNormalizer::normalize(name))
				.replace("@@Type@@", mApi.stringProperty(property, "type"));

		properties += propertyTemplate;
	}
	return properties;
}
