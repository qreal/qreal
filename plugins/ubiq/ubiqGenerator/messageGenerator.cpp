#include "messageGenerator.h"

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
	QString result;
	loadTemplateFromFile(fileName, result);
	loadTemplateUtils();

	QDir dir;
	if (!dir.exists(mOutputDirPath))
		dir.mkdir(mOutputDirPath);
	dir.cd(mOutputDirPath);

	foreach (Id const diagram, mApi.elementsByType("DataStructuresDiagram")) {
		if (!mApi.isLogicalElement(diagram))
			continue;

		foreach (Id const element, mApi.children(diagram)) {
			if (!mApi.isLogicalElement(element))
				continue;

			if (element.element() == "MessageClass") {
				result.replace("@@Properties@@", generatePropertiesCode(element));
			} else if (element.element() == "MessageCodes") {
				result = generateMessageCodes(result, element);
			} else if (element.element() == "ErrorCodes") {
				result = generateErrorCodes(result, element);
			}
		}
	}

	saveOutputFile(dir.absoluteFilePath(fileName), result);
}

QString MessageGenerator::generateMessageCodes(QString const &templateString, qReal::Id const &id)
{
	QString result = templateString;
	return result;
}

QString MessageGenerator::generateErrorCodes(QString const &templateString, qReal::Id const &id)
{
	QString result = templateString;
	return result;
}
