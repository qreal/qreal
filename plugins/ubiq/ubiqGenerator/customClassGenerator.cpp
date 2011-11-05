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
	foreach (Id const property, mApi.children(element)){

		// generate property code

		qDebug() << property.toString();
		QString propertyTemplate = mTemplateUtils["@@Property@@"];
		QString name = mApi.name(property);
		propertyTemplate.replace("@Name@", NameNormalizer::normalize(name, false))
				.replace("@NameCaps@", NameNormalizer::normalize(name))
				.replace("@Type@", mApi.stringProperty(property, "type"));


		properties += propertyTemplate;
	}
	return properties;
}


void CustomClassGenerator::generate()
{
	loadTemplateUtils();

	foreach (Id const diagram, mApi.elementsByType("DataStructuresDiagram")){ // for each diagram
		if (!mApi.isLogicalElement(diagram))
			continue;

		// get custom classes

		foreach (Id const element, mApi.children(diagram)){
			if (!mApi.isLogicalElement(element) || (element.element() != customClassLabel))
				continue;

			QString const templateName = "CustomClass.cs";

			loadTemplateFromFile(templateName, mFileTemplate);
			mFileTemplate.replace("@@CustomClassName@@", mApi.name(element));

			QString properties = generatePropertiesCode(element);
			mFileTemplate.replace("@@Properties@@", properties).replace("\\n", "\n");

			saveOutputFile(NameNormalizer::normalize(mApi.name(element)) + ".cs", mFileTemplate);
		}
	}
}
