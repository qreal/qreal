#include "commonGeneratorInfo.h"

using namespace constraints::generator::generatorUtils;
using namespace qReal;

CommonGeneratorInfo::CommonGeneratorInfo(const QMap<QString, QString> &templateUtils)
{
	QString diagramNameTemplate = mTemplateUtils["@@diagramName@@"];
	diagramNameTemplate = replaceLanguageName(diagramNameTemplate, diagram, count);
	diagramNameTemplate.truncate(diagramNameTemplate.size() - 1);

	QString hFilesTemplate = mTemplateUtils["@@constraintsDiagramHFile@@"];
	QString cppFilesTemplate = mTemplateUtils["@@constraintsDiagramCppFile@@"];
	hFilesTemplate.replace("@@diagramName@@", diagramNameTemplate);
	cppFilesTemplate.replace("@@diagramName@@", diagramNameTemplate);

	QString includeFilesTemplate = mTemplateUtils["@@includeConstraintsDiagramFile@@"];
	QString privateFieldsTemplate = mTemplateUtils["@@privateFieldOfConstraintsDigram@@"];
	includeFilesTemplate.replace("@@diagramName@@", diagramNameTemplate);
	privateFieldsTemplate.replace("@@diagramName@@", diagramNameTemplate);

	QString ifForMainCheckTemplate = mTemplateUtils["@@ifForMainCheckOfConstraintsDiagram@@"];
	ifForMainCheckTemplate.replace("@@diagramName@@", diagramNameTemplate);
	ifForMainCheckTemplate.replace("@@keywordForAllEdges@@", keywordForAllEdges);
	ifForMainCheckTemplate.replace("@@keywordForAllNodes@@", keywordForAllNodes);
	ifForMainCheckTemplate.replace("@@keywordForAllLanguages@@", keywordForAllLanguages);

	generateDiagramFiles(diagram, diagramNameTemplate);
	count++;

	hFiles += hFilesTemplate;
	cppFiles += cppFilesTemplate;

	includeFilesPluginH += includeFilesTemplate;
	privateFieldsPluginH += privateFieldsTemplate;

	ifForMainCheckPluginCPP += ifForMainCheckTemplate;
}

QString CommonGeneratorInfo::replaceLanguageName(QString string
		, const Id &diagram
		, const int count)
{
	string.replace("@@languageName@@", correctedLanguageName(diagram))
			.replace("@@id@@", QString::number(count));
	return string;
}

