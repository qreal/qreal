#pragma once

#include "myAbstractGenerator.h"

namespace generators {
namespace generator {

class StructuralGenerator : public MyAbstractGenerator
{
public:
	StructuralGenerator(QString const &templateDirPath
			, QString const &outputDirPath
			, QString const &pathToQReal
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			, QString const &metamodelLanguageName
			, QString const &languageName
			, QString const &nodeName
			, QString const &toGeneratePropertyName
			, QString const &programNamePropertyName
			, QString const &generatorMetamodelName
			);
	virtual ~StructuralGenerator();

	/// Starts generation.
	void generate();
	QString generateSemanticNodes();
	QString generateSemanticNode(qReal::Id const &element, QString const& templateName, int count, int parentCount = -1);
	QString generateForeachNode(qReal::Id const &element, int count, int parentCount);
	QString generateConverterNode(qReal::Id const &element, int count);

	QStringList marksList(QString const& text);
};

}
}

