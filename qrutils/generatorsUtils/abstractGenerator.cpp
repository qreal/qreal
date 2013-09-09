#include <QtCore/QFile>
#include <QtCore/QDebug>

#include "abstractGenerator.h"
#include "nameNormalizer.h"

using namespace generatorsUtils;
using namespace qReal;

AbstractGenerator::AbstractGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
		: mApi(logicalModel.logicalRepoApi())
		, mErrorReporter(errorReporter)
		, mOutputDirPath(outputDirPath)
		, mTemplateDirPath(templateDirPath)
{
}

AbstractGenerator::~AbstractGenerator()
{
}

QDir AbstractGenerator::getDir(QString const &path)
{
	QDir const result(path);
	if (!result.exists()) {
		qDebug() << "Cannot find directory " << path;
		return QDir();
	}

	return result;
}

bool AbstractGenerator::loadTemplateFromFile(QString const &templateFileName, QString &loadedTemplate)
{
	QDir const dir = getDir(mTemplateDirPath);
	if (dir == QDir()) {
		return false;
	}

	QString const fileName = dir.absoluteFilePath(templateFileName);
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		mErrorReporter.addError(QObject::tr("Cannot load file \"%1\"").arg(fileName));
		return false;
	}
	QTextStream in(&file);
	loadedTemplate = in.readAll();
	file.close();

	return true;
}

bool AbstractGenerator::loadUtilsFromFile()
{
	QDir const dir = getDir(mTemplateDirPath);
	if (dir == QDir()) {
		return false;
	}

	QString const fileName = dir.absoluteFilePath(utilsFileName);
	QFile utilsFile(fileName);
	if (!utilsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		mErrorReporter.addError(QObject::tr("Cannot load file \"%1\"").arg(fileName));
		return false;
	}
	QTextStream in(&utilsFile);

	QString line = in.readLine();
	do {
		// first line is name, everything else before the separator is template body
		QString const name = line;
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

	return true;
}

bool AbstractGenerator::loadUtilsFromDir()
{
	QDir const dir = getDir(mTemplateDirPath + "/utils");
	if (dir == QDir()) {
		return false;
	}

	QStringList const files = dir.entryList(QStringList());

	foreach (QString const &fileName, files) {
		if (fileName == "." || fileName == "..") {
			continue;
		}

		// file name is template name, file contents is template body
		QString const file = dir.absoluteFilePath(fileName);
		QFile templateFile(file);

		if (!templateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			mErrorReporter.addError(QObject::tr("Cannot load file \"%1\"").arg(file));
			return false;
		}

		QTextStream in(&templateFile);

		QString line = in.readLine();
		QString body = line;
		while (!line.isNull()) {
			body += (line + '\n');
			line = in.readLine();
		}

		QString const name = "@@" + fileName + "@@";
		if (!mTemplateUtils.contains(name)) {
			mTemplateUtils[name] = body;
		}

		templateFile.close();
	}

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

	if (!dir.exists(mOutputDirPath)) {
		QString messageText = QObject::tr("Cannot create directory %1").arg(mOutputDirPath);
		if (!dir.mkdir(mOutputDirPath)) {
			mErrorReporter.addUniqueError(messageText);
		} else {
			mErrorReporter.delUniqueError(messageText);
		}
	}
	dir.cd(mOutputDirPath);

	QString const outputFileName = dir.absoluteFilePath(fileName);
	QFile file(outputFileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		mErrorReporter.addError(QObject::tr("Cannot open \"%1\"").arg(outputFileName));
		return;
	}

	QTextStream out(&file);
	out << content;
	file.close();
}

QString AbstractGenerator::getDefaultValue(QString const &type)
{
	// Here to write all other needed types.
	if (type == "short" || type == "int") {
		return "0";
	}

	return "new " + type + "()";
}

QString AbstractGenerator::generatePropertiesCode(Id const &element)
{
	QString properties;
	foreach (Id const &property, mApi.children(element)) {
		if (!mApi.isLogicalElement(property) || property.element() != "Field") {
			continue;
		}

		// generate property code
		QString propertyTemplate = mTemplateUtils["@@Property@@"];
		QString const name = mApi.name(property);
		propertyTemplate.replace("@@Name@@", NameNormalizer::normalize(name))
				.replace("@@Type@@", mApi.stringProperty(property, "type"));

		properties += propertyTemplate;
	}
	return properties;
}
