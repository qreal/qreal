/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include <QtCore/QFile>

#include "customClassGenerator.h"
#include <qrutils/nameNormalizer.h>

using namespace ubiq::generator;
using namespace qReal;
using namespace utils;

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

	foreach (Id const &property, mApi.children(element)) {
		QString const name = NameNormalizer::normalize(mApi.name(property));
		QString const type = mApi.stringProperty(property, "type");

		// constructor that inits all fields
		QString propertyTemplate = mTemplateUtils["@@FieldInit@@"];
		QString argumentTemplate = mTemplateUtils["@@Argument@@"];
		QString const argName = NameNormalizer::normalize(mApi.name(property), false);
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
	loadUtilsTemplates();

	foreach (Id const &diagram, mApi.elementsByType("DataStructuresDiagram")) {
		if (!mApi.isLogicalElement(diagram)) {
			continue;
		}

		foreach (Id const &element, mApi.children(diagram)) {
			if (!mApi.isLogicalElement(element) || element.element() != customClassLabel) {
				continue;
			}

			QString const templateName = "CustomClass.cs";

			QString fileTemplate;
			loadTemplateFromFile(templateName, fileTemplate);

			QString const constructors = generateConstructors(element);
			QString const properties = generatePropertiesCode(element);

			fileTemplate.replace("@@Properties@@", properties)
					.replace("@@Constructors@@", constructors)
					.replace("@@CustomClassName@@", mApi.name(element));

			saveOutputFile(NameNormalizer::normalize(mApi.name(element)) + ".cs", fileTemplate);
		}
	}
}
