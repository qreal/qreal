#pragma once

#include "../../../qrutils/generatorsUtils/abstractGenerator.h"

namespace constraints {
namespace generator {

class ConcreateGenerator : public generatorsUtils::AbstractGenerator
{
public:
	/** Constructor.
	  @param templateDirPath Path to a directory with generation template.
	  @param outputDirPath Path to a directory where Message.cs shall be generated.
	  @param logicalModel Logical model reference.
	  @param errorReporter Object to return errors to.
	  */
	ConcreateGenerator(QString const &templateDirPath
			, QString const &outputDirPath
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			, QString const &metamodelName
			);

	virtual ~ConcreateGenerator();

	/// Starts generation.
	void generate();

private :
	struct NeededStringsForCommonGenerate {
		QString hFiles;
		QString cppFiles;
		QString includeFilesPluginH;
		QString privateFieldsPluginH;
		QString ifForMainCheckPluginCPP;

		NeededStringsForCommonGenerate(QString curHFiles, QString curCppFiles, QString curIncludeFilesPluginH
				, QString curPrivateFieldsPluginH, QString curIfForMainCheckPluginCPP)
				: hFiles(curHFiles), cppFiles(curCppFiles), includeFilesPluginH(curIncludeFilesPluginH)
				, privateFieldsPluginH(curPrivateFieldsPluginH), ifForMainCheckPluginCPP(curIfForMainCheckPluginCPP) {}
	};

	struct NeededStringsForConcreateGenerate {
		QString optionalChecksForElemetsH;
		QString mainChecksForElemetsH;
		QString countOptionalCheckStatusesForElemetsCPP;
		QString countMainCheckStatusesForElemetsCPP;
		QString returnCheckStatusesOfElementsInCheckCPP;
		QString addElementsInElementsNamesCPP;

		NeededStringsForConcreateGenerate(QString curOptionalChecksForElemetsH, QString curMainChecksForElemetsH
				, QString curCountOptionalCheckStatusesForElemetsCPP, QString curCountMainCheckStatusesForElemetsCPP
				, QString curReturnCheckStatusesOfElementsInCheckCPP, QString curAddElementsInElementsNamesCPP)
				: optionalChecksForElemetsH(curOptionalChecksForElemetsH), mainChecksForElemetsH(curMainChecksForElemetsH)
				, countOptionalCheckStatusesForElemetsCPP(curCountOptionalCheckStatusesForElemetsCPP)
				, countMainCheckStatusesForElemetsCPP(curCountMainCheckStatusesForElemetsCPP)
				, returnCheckStatusesOfElementsInCheckCPP(curReturnCheckStatusesOfElementsInCheckCPP)
				, addElementsInElementsNamesCPP(curAddElementsInElementsNamesCPP) {}
	};

	NeededStringsForConcreateGenerate generateCommonNeededPartsForElements(QString elementName);
	QString generateMainCheckStatusesForElemetsCPP(QMap<QString, QString> appendOptionalCheckStatusInMainCheckCPP);
	NeededStringsForConcreateGenerate generateNeededPartsForDiagramFiles(qReal::Id const &diagram);
	void generateDiagramFiles(qReal::Id const &diagram, QString diagramNameTemplate);
	NeededStringsForCommonGenerate generateNeededPartsForAllConstraintsDiagram();
	QString replaceLanguageName(QString string, qReal::Id const &diagram, int count);

	QString countRealConstraintOfElement(qReal::Id const &element);
	QString countRealConstraintForEdgeElement(qReal::Id const &element, QString elementName, QString resultName, int depth, QString addStr);
	QString countRealConstraintForNodeElement(qReal::Id const &element, QString elementName, QString resultName, int depth, QString addStr);
	QString additionalCommonPartForConstraint(QList<QString> resBool, QString resultName, int depth, QString addStr);

	QPair<QString, QList<QString> > countConstraintForBeginNode(qReal::Id const &constraint, QString elementName, int depth, QString addStr);
	QPair<QString, QList<QString> > countConstraintForEndNode(qReal::Id const &constraint, QString elementName, int depth, QString addStr);

	QPair<QString, QList<QString> > countConstraintForListOfElements(qReal::Id const &constraint, QString elementName, QString resElementName, QString functionName, QString resType, int depth, QString addStr);

	QPair<QString, QList<QString> > countConstraintForParent(qReal::Id const &constraint, QString elementName, int depth, QString addStr);
	QPair<QString, QList<QString> > countConstraintForChildrens(qReal::Id const &constraint, QString elementName, int depth, QString addStr);
	QPair<QString, QList<QString> > countConstraintForOutgoingLinks(qReal::Id const &constraint, QString elementName, int depth, QString addStr);
	QPair<QString, QList<QString> > countConstraintForIncomingLinks(qReal::Id const &constraint, QString elementName, int depth, QString addStr);
	QPair<QString, QList<QString> > countConstraintForOutgoingNodes(qReal::Id const &constraint, QString elementName, int depth, QString addStr);
	QPair<QString, QList<QString> > countConstraintForIncomingNodes(qReal::Id const &constraint, QString elementName, int depth, QString addStr);
	QPair<QString, QList<QString> > countConstraintForPropertyNode(qReal::Id const &constraint, QString elementName, int depth, QString addStr);

	QString const &mMetamodelName;
	QString mCallingPath;

};

}
}

