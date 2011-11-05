#include "messageGenerator.h"

#include "nameNormalizer.h"

#include <QtCore/QFile>

#include <QtCore/QDebug>

using namespace ubiq::generator;
using namespace qReal;

QString const fileName = "Message.cs";

MessageGenerator::MessageGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
		: AbstractGenerator(templateDirPath, outputDirPath, logicalModel, errorReporter)
{
}

MessageGenerator::~MessageGenerator()
{
}

void MessageGenerator::generate()
{
	loadTemplateFromFile(fileName, mFileTemplate);
	loadTemplateUtils();

	QDir dir;
	if (!dir.exists(mOutputDirPath))
		dir.mkdir(mOutputDirPath);
	dir.cd(mOutputDirPath);

	foreach (Id const diagram, mApi.elementsByType("DataStructuresDiagram")) {
		if (!mApi.isLogicalElement(diagram))
			continue;

		foreach (Id const element, mApi.children(diagram)) {
			if (!mApi.isLogicalElement(element) || element.element() != "MessageClass")
				continue;

			mFileTemplate = generateCustomProperties(mFileTemplate, element);
		}
	}

	QString const outputFileName = dir.absoluteFilePath(fileName);
	QFile resultFile(outputFileName);
	if (!resultFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + resultFile.fileName() + "\"");
		return;
	}

	QTextStream out(&resultFile);
	out << mFileTemplate;
	out.flush();
	resultFile.close();
}

QString MessageGenerator::generateCustomProperties(QString const &templateString, qReal::Id const &id)
{
	QString properties;
	foreach (Id const element, mApi.children(id)) {
		if (!mApi.isLogicalElement(element) || element.element() != "Field")
			continue;

		QString propertyTemplate = mTemplateUtils["@@Property@@"];
		QString const name = mApi.name(element);
		propertyTemplate.replace("@Name@", NameNormalizer::normalize(name, false))
				.replace("@NameCaps@", NameNormalizer::normalize(name))
				.replace("@Type@", mApi.stringProperty(element, "type"));

		properties += propertyTemplate;
	}

	QString result = templateString;
	result.replace("@@Properties@@", properties);
	return result;
}
