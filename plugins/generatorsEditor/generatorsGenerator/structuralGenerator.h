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

private:
	enum TemplateType {
		file
		, mark
		, diagramMark // особая метка, использующая свойства элементы основной диаграммы (нужна генерация в plugin.cpp)
	};

	QMap<QString, QString> mSrcNodeToEdgeSemantic;
	QMap<QString, QString> mDstNodeToEdgeSemantic;

	QString generateSemanticNodes();
	QString generateSemanticNode(qReal::Id const &element, QString const& templateName, QString const& count, QString const& parentCount = "");
	QString generateForeachNode(qReal::Id const &element, QString const& count, QString const& parentCount);
	QString generateConverterNode(qReal::Id const &element, QString const& count);

	QString generateSemanticNodesForEdge();
	QString generateSemanticNodeForEdge(qReal::Id const &element, QString const& count);
	QString generateOneDstBody(qReal::Id const &element, QString const& count, QString const& dst);
	QString makeStringFromList(QStringList const& list);

	QString doReplaceForPropertyMarks(QString const& text, QString const& propertyMark);
	QString generateReplaceTemplateBodyForMark(QString const& textCode, QString const& elementMarkName, QString const& count);
	QString generateReplaceTemplateBodyForDiagramMark(QString const& textCode, QString const& elementMarkName);
	QString generateReplaceTemplateBodyForFile(QString const& textCode, QString const& templateVariableName, QString const& templateFileName);
	QPair<QString, TemplateType> generateReplaceTemplateBodyForTemplateNode(qReal::Id const &element, QString const& count);
	QPair<QString, QString> generateReplaceTemplateBody(qReal::Id const &element, QString const& count); // возращает код для generator.cpp и plugin.cpp

	QStringList marksList(QString const& text, QString const& mark);
	QPair<QString, QString> addTemplateString(QPair<QString, QString> sum, QPair<QString, TemplateType> str);
};

}
}

