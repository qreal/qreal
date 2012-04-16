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
	QString replaceLanguageName(QString string, qReal::Id const &diagram, int count);
	QString countRealConstraintOfElement(qReal::Id const &element);
	QString countRealConstraintForEdgeElement(qReal::Id const &element, QString elementName, QString resultName, int depth, QString addStr);
	QString countRealConstraintForNodeElement(qReal::Id const &element, QString elementName, QString resultName, int depth, QString addStr);
	QString const &mMetamodelName;
	QString mCallingPath;
};

}
}

