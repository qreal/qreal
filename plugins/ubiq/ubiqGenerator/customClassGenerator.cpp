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

QString CustomClassGenerator::generateConstructors(qReal::Id const &element)
{
	QString defaultConstructorTemplate = mTemplateUtils["@@Constructor@@"];
	QString constructorTemplate = defaultConstructorTemplate;

	defaultConstructorTemplate.replace("@@ConstructorParameters@@", "");

	QString parametersList;
	QString defaultProperties;
	QString properties;

	foreach (Id const property, mApi.children(element)) {
		QString name = NameNormalizer::normalize(mApi.name(property));
		QString type = mApi.stringProperty(property, "type");

		// constructor that inits all fields
		QString propertyTemplate = mTemplateUtils["@@FieldInit@@"];
		QString argumentTemplate = mTemplateUtils["@@Argument@@"];
		QString argName = NameNormalizer::normalize(mApi.name(property), false);
		argumentTemplate.replace("@@ArgType@@", type).replace("@@ArgName@@", argName);
		parametersList += (argumentTemplate.trimmed() + ", ");

		propertyTemplate.replace("@@Name@@", name).replace("@@Value@@", argName);
		properties += propertyTemplate;

		// default constructor
		QString defaultPropertyTemplate = mTemplateUtils["@@FieldInit@@"];
		defaultPropertyTemplate.replace("@@Name@@", name).replace("@@Value@@", getDefaultValue(type));
		defaultProperties += defaultPropertyTemplate;

	}

	parametersList.chop(2); // remove terminating space and comma
	constructorTemplate.replace("@@ConstructorParameters@@", parametersList)
			.replace("@@Fields@@", properties);


	defaultConstructorTemplate.replace("@@Fields@@", defaultProperties);
	return defaultConstructorTemplate + constructorTemplate;
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
