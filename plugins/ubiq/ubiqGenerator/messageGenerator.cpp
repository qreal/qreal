#include "messageGenerator.h"

#include "../../../qrutils/generatorsUtils/nameNormalizer.h"

#include <QtCore/QFile>

#include <QtCore/QDebug>

using namespace ubiq::generator;
using namespace qReal;
using namespace generatorsUtils;

/// Generation target file
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
	loadUtilsTemplates();

	foreach (Id const &diagram, mApi.elementsByType("DataStructuresDiagram")) {
		if (!mApi.isLogicalElement(diagram)) {
			continue;
		}

		foreach (Id const &element, mApi.children(diagram)) {
			if (!mApi.isLogicalElement(element)) {
				continue;
			}

			if (element.element() == "MessageClass") {
				result.replace("@@Properties@@", generatePropertiesCode(element))
						.replace("@@InitFieldsWithDefaults@@", generateDefaultFieldsInitialization(element))
						.replace("@@ConstructorArgs@@", generateConstructorArguments(element))
						.replace("@@InitFieldsWithArgs@@", generateFieldsInitialization(element))
						.replace("@@ConstructorActualArgs@@", generateConstructorActualArguments(element))
						.replace("@@PackFields@@", generatePackFields(element))
						.replace("@@UnpackFields@@", generateUnpackFields(element))
						;
			} else if (element.element() == "MessageCodes") {
				result.replace("@@MessageCodes@@", generateEnumElements(element));
			} else if (element.element() == "ErrorCodes") {
				result.replace("@@ErrorCodes@@", generateEnumElements(element));
			}
		}
	}

	saveOutputFile(fileName, result);
}

QString MessageGenerator::generateEnumElements(qReal::Id const &element) const
{
	QString result;
	foreach (Id const &id, mApi.children(element)) {
		if (!mApi.isLogicalElement(id) || id.element() != "EnumElement") {
			continue;
		}

		QString propertyTemplate = mTemplateUtils["@@EnumElement@@"];
		QString const name = mApi.name(id);
		QString const value = mApi.stringProperty(id, "value");
		propertyTemplate.replace("@@Name@@", NameNormalizer::normalize(name))
				.replace("@@Value@@", value);

		result += propertyTemplate;
	}
	return result;
}

QString MessageGenerator::generateDefaultFieldsInitialization(qReal::Id const &element) const
{
	QString fieldsInitialization;
	foreach (Id const &property, mApi.children(element)) {
		if (!mApi.isLogicalElement(property) || property.element() != "Field") {
			continue;
		}

		QString initTemplate = mTemplateUtils["@@FieldInit@@"];
		QString const name = mApi.name(property);

		QString const defaultValue = mApi.stringProperty(property, "defaultValue").isEmpty()
				? getDefaultValue(mApi.stringProperty(property, "type"))
				: mApi.stringProperty(property, "defaultValue");

		initTemplate.replace("@@Name@@", name)
				.replace("@@Value@@", defaultValue)
				;

		fieldsInitialization += ("    " + initTemplate);
	}
	return fieldsInitialization;
}

QString MessageGenerator::generateFieldsInitialization(qReal::Id const &element) const
{
	QString fieldsInitialization;
	foreach (Id const &property, mApi.children(element)) {
		if (!mApi.isLogicalElement(property) || property.element() != "Field") {
			continue;
		}

		QString initTemplate = mTemplateUtils["@@FieldInit@@"];
		QString argName = mApi.name(property);
		initTemplate.replace("@@Name@@", argName)
				.replace("@@Value@@", NameNormalizer::normalize(argName, false))
				;

		fieldsInitialization += ("    " + initTemplate);
	}
	return fieldsInitialization;
}

QString MessageGenerator::generateConstructorArguments(qReal::Id const &element) const
{
	QString parametersList;
	foreach (Id const &property, mApi.children(element)) {
		if (!mApi.isLogicalElement(property) || property.element() != "Field") {
			continue;
		}

		QString argumentTemplate = mTemplateUtils["@@Argument@@"];
		QString argName = NameNormalizer::normalize(mApi.name(property), false);
		QString type = mApi.stringProperty(property, "type");
		argumentTemplate.replace("@@ArgType@@", type).replace("@@ArgName@@", argName);
		parametersList += (argumentTemplate.trimmed() + ", ");
	}
	return parametersList;
}

QString MessageGenerator::generateConstructorActualArguments(qReal::Id const &element) const
{
	QString parametersList;
	foreach (Id const &property, mApi.children(element)) {
		if (!mApi.isLogicalElement(property) || property.element() != "Field") {
			continue;
		}

		QString argName = NameNormalizer::normalize(mApi.name(property), false);
		parametersList += (argName + ", ");
	}
	return parametersList;
}

QString MessageGenerator::generatePackFields(qReal::Id const &element) const
{
	return generateSerializationRelatedCode(element, "Serialization");
}

QString MessageGenerator::generateUnpackFields(qReal::Id const &element) const
{
	return generateSerializationRelatedCode(element, "Deserialization");
}

QString MessageGenerator::generateSerializationRelatedCode(qReal::Id const &element, QString const &method) const
{
	QString serializersList;
	foreach (Id const &property, mApi.children(element)) {
		if (!mApi.isLogicalElement(property) || property.element() != "Field") {
			continue;
		}

		if (!mApi.property(property, "serialize").toBool()) {
			continue;
		}

		QString const name = mApi.name(property);
		QString const type = mApi.stringProperty(property, "type");

		QString serializationTemplate;

		if (type == "int") {
			bool const serializeAsShort = mApi.property(property, "serializeAsShort").toBool();
			if (serializeAsShort) {
				serializationTemplate = mTemplateUtils["@@" + method + "IntAsShort@@"];
			}
		}

		if (serializationTemplate.isEmpty()) {
			serializationTemplate = mTemplateUtils["@@" + method + "_" + type + "@@"];
		}

		serializationTemplate.replace("@@Name@@", name);
		serializersList += serializationTemplate;
	}
	return serializersList;
}
