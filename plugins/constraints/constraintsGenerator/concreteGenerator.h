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

#pragma once

#include "generatorUtils/defs.h"

#include <qrutils/generator/abstractGenerator.h>

namespace constraints {
namespace generator {

/// This class is responsible for code generation for constraints.
class ConcreteGenerator : public utils::AbstractGenerator
{
public:
	/// Constructor.
	/// @param templateDirPath - path to a directory with generation template.
	/// @param outputDirPath - path to a directory.
	/// @param pathToQReal - path to QReal sources.
	/// @param logicalModel - logical model reference.
	/// @param errorReporter - object to return errors to.
	/// @param metamodelLanguageName - metamodel language name.
	/// @param constraintsMetamodelName - name of constraints metamodel.
	ConcreteGenerator(
			const QString &templateDirPath
			, const QString &outputDirPath
			, const QString &pathToQReal
			, const qReal::LogicalModelAssistInterface &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			, const QString &metamodelLanguageName
			, const QString &constraintsMetamodelName
			);

	~ConcreteGenerator() override;

	void generate() override;

	/// Returns full name of constraint model.
	QString constraintModelFullName() const;

	/// Returns name of constraints model.
	QString constraintModelName() const;

	QString constraintConstraintsModelName() const;
	QString normalizedConstraintModelName() const;
	QString normalizedConstraintConstraintsModelName() const;
	QString constraintModelId() const;

private :
	struct NeededStringsForConcreteGenerate {
		QString optionalChecksForElementsH;
		QString mainChecksForElementsH;
		QString countOptionalCheckStatusesForElementsCPP;
		QString countMainCheckStatusesForElementsCPP;
		QString prefixForReturnCheckStatusesOfElementsInCheckCPP;
		QString returnCheckStatusesOfElementsInCheckCPP;
		QString addElementsInElementsNamesCPP;

		NeededStringsForConcreteGenerate(
				QString curOptionalChecksForElementsH
				, QString curMainChecksForElementsH
				, QString curCountOptionalCheckStatusesForElementsCPP
				, QString curCountMainCheckStatusesForElementsCPP
				, QString curPrefixForReturnCheckStatusesOfElementsInCheckCPP
				, QString curReturnCheckStatusesOfElementsInCheckCPP
				, QString curAddElementsInElementsNamesCPP)
			: optionalChecksForElementsH(curOptionalChecksForElementsH)
			, mainChecksForElementsH(curMainChecksForElementsH)
			, countOptionalCheckStatusesForElementsCPP(curCountOptionalCheckStatusesForElementsCPP)
			, countMainCheckStatusesForElementsCPP(curCountMainCheckStatusesForElementsCPP)
			, prefixForReturnCheckStatusesOfElementsInCheckCPP(curPrefixForReturnCheckStatusesOfElementsInCheckCPP)
			, returnCheckStatusesOfElementsInCheckCPP(curReturnCheckStatusesOfElementsInCheckCPP)
			, addElementsInElementsNamesCPP(curAddElementsInElementsNamesCPP)
		{}
	};

	void generateCodeForProFile();
	void generateCodeForConstraintsPluginHFile();
	void generateCodeForConstraintsPluginCppFile();

	QString templateWithReplacedDiagramName(const QString &templateName
			, const QString &diagramName) const;

	QString generatedPartOfTemplateForAllDiagrams(const QString &templateName
			, const bool isIfForMainCheck = false);

	NeededStringsForConcreteGenerate generateCommonNeededPartsForElements(const QString &elementName);
	QString generateMainCheckStatusesForElementsCPP(QMap<QString, QString> appendOptionalCheckStatusInMainCheckCPP);
	NeededStringsForConcreteGenerate generateNeededPartsForDiagramFiles(const qReal::Id &diagram);
	void generateDiagramFiles(const qReal::Id &diagram, const QString &diagramNameTemplate);
	QString replaceLanguageName(QString string, const qReal::Id &diagram, const int count);
	QString correctedLanguageName(const qReal::Id &diagram);

	QString mPathToQReal;
	QString const &mMetamodelName; //имя метамодели языков, для которых напсана эта
			//модель ограничений, по которой генерируется код
	QString const &mConstraintsName;//настоящее имя модели огранчений, по которой генерируется код

	bool mUsedMetaTypeInCheck; // для генерации ограничений множества элементов в Check;
							// true = надо использвать мета-тип элемента; false = не надо использовать;
	QMap<QString, int> mCountsOfConstraintElementsInOneConstraint;

};

}
}

