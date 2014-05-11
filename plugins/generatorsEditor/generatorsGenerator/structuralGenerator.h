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
	QString generateSemanticNode(qReal::Id const &element, QString const& templateName, QString const& count, QString const& parentCount = "");
	QString generateForeachNode(qReal::Id const &element, QString const& count, QString const& parentCount);
	QString generateConverterNode(qReal::Id const &element, QString const& count);

	QString generateReplaceTemplateBodyForMark(QString const& textCode, QString const& elementMarkName, QString const& count);
	QString generateReplaceTemplateBodyForFile(QString const& textCode, QString const& templateVariableName, QString const& templateFileName);
	QString generateReplaceTemplateBodyForTemplateNode(qReal::Id const &element, QString const& count);
	QString generateReplaceTemplateBody(qReal::Id const &element, QString const& count);
	QStringList marksList(QString const& text, QString const& mark);
};

}
}

