#pragma once

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
	QString constraintNormalizerModelName() const; //i.e. fileBase
	QString constraintNormalizerConstraintsModelName() const; //i.e. normalizerPluginName
	QString constraintModelId() const; //i.e. pluginId

private :

	enum metaType {
		node,
		edge,
		empty
	};

	struct NeededStringsForCommonGenerate {
		NeededStringsForCommonGenerate(const QString &curHFiles
				, const QString &curCppFiles
				, const QString &curIncludeFilesPluginH
				, const QString &curPrivateFieldsPluginH
				, const QString &curIfForMainCheckPluginCPP)
			: mHFiles(curHFiles)
			, mCppFiles(curCppFiles)
			, mIncludeFilesPluginH(curIncludeFilesPluginH)
			, mPrivateFieldsPluginH(curPrivateFieldsPluginH)
			, mIfForMainCheckPluginCPP(curIfForMainCheckPluginCPP)
		{}

		QString mHFiles;
		QString mCppFiles;
		QString mIncludeFilesPluginH;
		QString mPrivateFieldsPluginH;
		QString mIfForMainCheckPluginCPP;
	};

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

	NeededStringsForConcreteGenerate generateCommonNeededPartsForElements(QString elementName);
	QString generateMainCheckStatusesForElementsCPP(QMap<QString, QString> appendOptionalCheckStatusInMainCheckCPP);
	NeededStringsForConcreteGenerate generateNeededPartsForDiagramFiles(qReal::Id const &diagram);
	void generateDiagramFiles(qReal::Id const &diagram, QString diagramNameTemplate);
	NeededStringsForCommonGenerate generateNeededPartsForAllConstraintsDiagram();
	QString replaceLanguageName(QString string, qReal::Id const &diagram, int count);
	QString correctedLanguageName(qReal::Id const &diagram);

	QString countRealConstraintOfDiagramElement(qReal::Id const &constraintElement);
	QString countRealConstraintForElement(qReal::Id const &constraintElement, metaType const &type, QString elementName, QString resultName, int depth, QString addStr);
	QString countRealConstraintForEdgeElement(qReal::Id const &constraintElement, QString elementName, QString resultName, int depth, QString addStr);
	QString countRealConstraintForNodeElement(qReal::Id const &constraintElement, QString elementName, QString resultName, int depth, QString addStr);
	QPair<QString, QList<QString> > countRealConstraintForOneEdgeElement(qReal::Id const &constraint, qReal::IdList &usedElements, QString elementName, int depth, QString addStr, bool isMultiOr = false);
	QPair<QString, QList<QString> > countRealConstraintForOneNodeElement(qReal::Id const &constraint, qReal::IdList &usedElements, QString elementName, int depth, QString addStr, bool isMultiOr = false);
	QPair<QString, QList<QString> > countNeighborsElementsByOr(qReal::Id const &constraint, QString resConstraintBool, qReal::IdList &usedElements, metaType const &type, QString elementName, int depth, QString addStr);
	QPair<bool, QString> handleConstraintsSelection(qReal::Id const &constraintElement);
	QString additionalCommonPartForConstraint(QList<QString> resBool, QString resultName, int depth, QString addStr);

	QString pushResBoolInResStringByAnd(QList<QString> resBool);
	QString pushResBoolInResStringByOr(QList<QString> resBool);
	qReal::IdList neighborNodesByType(qReal::Id const &element, QString const &type);
	bool linksContainsByType(qReal::Id const &element, QString const &type);

	QPair<QString, QList<QString> > countConstraintForBeginNode(qReal::Id const &constraint, QString elementName, int depth, QString addStr);
	QPair<QString, QList<QString> > countConstraintForEndNode(qReal::Id const &constraint, QString elementName, int depth, QString addStr);

	QPair<QString, QList<QString> > countConstraintForListOfElements(qReal::Id const &constraint, QString elementName, QString resElementName, QString functionName, QString resType, int depth, QString addStr);
	QString generateExistsProperty(QString const &resElementName, QString const &elementName, qReal::Id const &constraint, int depth, QString addStr);

	QPair<QString, QList<QString> > countConstraintForParent(qReal::Id const &constraint, QString elementName, int depth, QString addStr);
	QPair<QString, QList<QString> > countConstraintForChildrens(qReal::Id const &constraint, QString elementName, int depth, QString addStr);
	QPair<QString, QList<QString> > countConstraintForOutgoingLinks(qReal::Id const &constraint, QString elementName, int depth, QString addStr);
	QPair<QString, QList<QString> > countConstraintForIncomingLinks(qReal::Id const &constraint, QString elementName, int depth, QString addStr);
	QPair<QString, QList<QString> > countConstraintForOutgoingNodes(qReal::Id const &constraint, QString elementName, int depth, QString addStr);
	QPair<QString, QList<QString> > countConstraintForIncomingNodes(qReal::Id const &constraint, QString elementName, int depth, QString addStr);
	QPair<QString, QList<QString> > countConstraintForPropertyNode(qReal::Id const &constraint, QString elementName, int depth, QString addStr);
	QPair<QString, QString > countPropertyCharacteristicForConstraintElement(qReal::Id const &constraint, QString const &characteristicName, QString const &defaultValue, QString property, QString sign, QString value,  QString elementName, int depth, QString addStr);
	QPair<QString, QList<QString> > countConstraintForMultiOrNode(qReal::Id const &constraint, qReal::IdList &usedElements, metaType const &type, QString elementName, int depth, QString addStr);

	QString mPathToQReal;
	QString const &mMetamodelName;//имя метамодели языков, для которых напсана эта модель ограничений, по которой генерируется код
	QString const &mConstraintsName;//настоящее имя модели огранчений, по которой генерируется код

	bool mUsedMetaTypeInCheck; // для генерации ограничений множества элементов в Check;
							// true = надо использвать мета-тип элемента; false = не надо использовать;
	QMap<QString, int> mCountsOfConstraintElementsInOneConstraint;

};

}
}

