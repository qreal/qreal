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

	QString countRealConstraintOfDiagramElement(const qReal::Id &constraintElement);

	QString countRealConstraintForElement(
			const qReal::Id &constraintElement
			, const metaType &type
			, const QString &elementName
			, const QString &resultName
			, const int depth
			, const QString &addStr);

	QString countRealConstraintForEdgeElement(
			const qReal::Id &constraintElement
			, const QString &elementName
			, const QString &resultName
			, int depth
			, const QString &addStr);

	QString countRealConstraintForNodeElement(
			const qReal::Id &constraintElement
			, const QString &elementName
			, const QString &resultName
			, const int depth
			, const QString &addStr);

	QPair<QString, QList<QString> > countRealConstraintForOneEdgeElement(
			const qReal::Id &constraint
			, qReal::IdList &usedElements
			, const QString &elementName
			, const int depth
			, const QString &addStr
			, const bool isMultiOr = false);

	QPair<QString, QList<QString> > countRealConstraintForOneNodeElement(
			const qReal::Id &constraint
			, qReal::IdList &usedElements
			, const QString &elementName
			, const int depth
			, const QString &addStr
			, const bool isMultiOr = false);

	QPair<QString, QList<QString> > countNeighborsElementsByOr(
			const qReal::Id &constraint
			, const QString &resConstraintBool
			, qReal::IdList &usedElements
			, const metaType &type
			, const QString &elementName
			, const int depth
			, const QString &addStr);

	QPair<bool, QString> handleConstraintsSelection(const qReal::Id &constraintElement);

	QString additionalCommonPartForConstraint(
			const QList<QString> &resBool
			, const QString &resultName
			, const int depth
			, const QString &addStr);

	QPair<QString, QList<QString> > countConstraintForBeginNode(
			qReal::Id const &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString);

	QPair<QString, QList<QString> > countConstraintForEndNode(
			qReal::Id const &constraint
			, const QString &elementName
			, int depth
			, const QString &additionalString);

	QPair<QString, QList<QString> > countConstraintForParent(
			const qReal::Id &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString);

	QPair<QString, QList<QString> > countConstraintForPropertyNode(
			qReal::Id const &constraint
			, const QString &elementName
			, const int depth
			, const QString &addStr);

	QPair<QString, QString > countPropertyCharacteristicForConstraintElement(const qReal::Id &constraint
			, const QString &characteristicName
			, const QString &defaultValue
			, const QString &property
			, QString sign
			, QString value
			, const QString &elementName
			, const int depth
			, const QString &addStr);

	QPair<QString, QList<QString> > countConstraintForMultiOrNode(
			const qReal::Id &constraint
			, qReal::IdList &usedElements
			, const metaType &type
			, const QString &elementName
			, const int depth
			, const QString &addStr);

	QString mPathToQReal;
	QString const &mMetamodelName; //имя метамодели языков, для которых напсана эта модель ограничений, по которой генерируется код
	QString const &mConstraintsName;//настоящее имя модели огранчений, по которой генерируется код

	bool mUsedMetaTypeInCheck; // для генерации ограничений множества элементов в Check;
							// true = надо использвать мета-тип элемента; false = не надо использовать;
	QMap<QString, int> mCountsOfConstraintElementsInOneConstraint;

};

}
}

