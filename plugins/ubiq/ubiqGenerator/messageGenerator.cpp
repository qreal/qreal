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
				result.replace("@@MessageCodes@@", generateEnumElements(element));
			} else if (element.element() == "ErrorCodes") {
				result.replace("@@ErrorCodes@@", generateEnumElements(element));
			}
		}
	}

	saveOutputFile(dir.absoluteFilePath(fileName), result);
}

QString MessageGenerator::generateEnumElements(qReal::Id const &element)
{
	QString result;
	int value = 0;
	foreach (Id const id, mApi.children(element)) {
		if (!mApi.isLogicalElement(id) || id.element() != "EnumElement")
			continue;

		QString propertyTemplate = mTemplateUtils["@@EnumElement@@"];
		QString name = mApi.name(id);
		propertyTemplate.replace("@@Name@@", NameNormalizer::normalize(name))
				.replace("@@Value@@", QString::number(value));

		result += propertyTemplate;
		++value;
	}
	return result;
}
