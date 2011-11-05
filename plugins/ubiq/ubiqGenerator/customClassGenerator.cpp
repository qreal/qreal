#include <QtCore/QFile>
#include <QtCore/QDebug>

#include "customClassGenerator.h"
#include "nameNormalizer.h"

using namespace ubiq::generator;
using namespace qReal;

CustomClassGenerator::CustomClassGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
		: AbstractGenerator(templateDirPath , outputDirPath, logicalModel, errorReporter)
{
}

CustomClassGenerator::~CustomClassGenerator()
{
}

QString CustomClassGenerator::generatePropertiesCode(Id const &element)
{
	QString properties;
	foreach (Id const property, mApi.children(element)) {

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

QString CustomClassGenerator::generateConstructors(qReal::Id const &element)
{
	QString defaultConstructorTemplate = mTemplateUtils["@@Constructor@@"];
	QString constructorTemplate = defaultConstructorTemplate;

	defaultConstructorTemplate.replace("@@ConstructorParameters@@", "");

	QString properties;
	foreach (Id const property, mApi.children(element)) {
		QString name = NameNormalizer::normalize(mApi.name(property));
		QString type = mApi.stringProperty(property, "type"); // FIXME: not needed?
		QString propertyTemplate = mTemplateUtils["@@FieldInit@@"];
		propertyTemplate.replace("@@Name@@", name);

		QString defaultPropertyTemplate = propertyTemplate;

		defaultPropertyTemplate.replace("@@DefaultValue@@", getDefaultValue(mApi.stringProperty(property, "type")));
		properties += defaultPropertyTemplate;
	}

	defaultConstructorTemplate.replace("@@Fields@@", properties);
	return defaultConstructorTemplate;
}

void CustomClassGenerator::generate()
{
	loadTemplateUtils();

	foreach (Id const diagram, mApi.elementsByType("DataStructuresDiagram")) { // for each diagram
		if (!mApi.isLogicalElement(diagram))
			continue;

		// get custom classes

		foreach (Id const element, mApi.children(diagram)) {
			if (!mApi.isLogicalElement(element) || (element.element() != customClassLabel))
				continue;

			QString const templateName = "CustomClass.cs";

			QString fileTemplate;
			loadTemplateFromFile(templateName, fileTemplate);

			QString constructors = generateConstructors(element);
			QString properties = generatePropertiesCode(element);

			fileTemplate.replace("@@Properties@@", properties)
					.replace("@@Constructors@@", constructors)
					.replace("@@CustomClassName@@", mApi.name(element));

			saveOutputFile(NameNormalizer::normalize(mApi.name(element)) + ".cs", fileTemplate);
		}
	}
}
