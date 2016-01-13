#pragma once

#include "generatorUtils/defs.h"

#include <qrutils/generator/abstractGenerator.h>

namespace constraints {
namespace generator {

class ConcreteGenerator : public utils::AbstractGenerator
{
public:
	/** Constructor.
	  @param templateDirPath - Path to a directory with generation template.
	  @param outputDirPath - Path to a directory.
	  @param logicalModel - Logical model reference.
	  @param errorReporter - Object to return errors to.
	  */
	ConcreteGenerator(const QString &templateDirPath
			, const QString &outputDirPath
			, const QString &pathToQReal
			, const qReal::LogicalModelAssistInterface &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			, const QString &metamodelLanguageName
			, const QString &constraintsMetamodelName
			);

	~ConcreteGenerator() override;

	void generate() override;

	QString constraintModelFullName() const;
	QString constraintModelName() const;
	QString constraintConstraintsModelName() const; //i.e. pluginName
	QString normalizedConstraintModelName() const; //i.e. fileBase
	QString normalizedConstraintConstraintsModelName() const; //i.e. normalizerPluginName
	QString constraintModelId() const; //i.e. pluginId

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
	QString const &mMetamodelName; //имя метамодели языков, для которых напсана эта модель ограничений, по которой генерируется код
	QString const &mConstraintsName;//настоящее имя модели огранчений, по которой генерируется код

	bool mUsedMetaTypeInCheck; // для генерации ограничений множества элементов в Check;
							// true = надо использвать мета-тип элемента; false = не надо использовать;
	QMap<QString, int> mCountsOfConstraintElementsInOneConstraint;

};

}
}

