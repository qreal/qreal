#include "commonGeneratorInfo.h"
#include "defs.h"

using namespace constraints::generator::generatorUtils;
using namespace qReal;

CommonGeneratorInfo::CommonGeneratorInfo(const QMap<QString, QString> &templateUtils)
	: mTemplateUtils(templateUtils)
{
}

void CommonGeneratorInfo::fillOneDiagram(const Id &diagram)
{
	QString diagramNameTemplate = mTemplateUtils["@@diagramName@@"];
//	diagramNameTemplate = replaceLanguageName(diagramNameTemplate, diagram, count);
	diagramNameTemplate.truncate(diagramNameTemplate.size() - 1);

	QString hFilesTemplate = mTemplateUtils["@@constraintsDiagramHFile@@"];
	hFilesTemplate.replace("@@diagramName@@", diagramNameTemplate);

	QString cppFilesTemplate = mTemplateUtils["@@constraintsDiagramCppFile@@"];
	cppFilesTemplate.replace("@@diagramName@@", diagramNameTemplate);

	QString includeFilesTemplate = mTemplateUtils["@@includeConstraintsDiagramFile@@"];
	includeFilesTemplate.replace("@@diagramName@@", diagramNameTemplate);

	QString privateFieldsTemplate = mTemplateUtils["@@privateFieldOfConstraintsDigram@@"];
	privateFieldsTemplate.replace("@@diagramName@@", diagramNameTemplate);

	QString ifForMainCheckTemplate = mTemplateUtils["@@ifForMainCheckOfConstraintsDiagram@@"];

	ifForMainCheckTemplate.replace("@@diagramName@@", diagramNameTemplate);
	ifForMainCheckTemplate.replace("@@keywordForAllEdges@@", keywordForAllEdges);
	ifForMainCheckTemplate.replace("@@keywordForAllNodes@@", keywordForAllNodes);
	ifForMainCheckTemplate.replace("@@keywordForAllLanguages@@", keywordForAllLanguages);

	mHFiles += hFilesTemplate;
	mCppFiles += cppFilesTemplate;

	mIncludeFilesPluginH += includeFilesTemplate;
	mPrivateFieldsPluginH += privateFieldsTemplate;

	mIfForMainCheckPluginCPP += ifForMainCheckTemplate;
}

QString CommonGeneratorInfo::hFilesString() const
{
	return mHFiles;
}

QString CommonGeneratorInfo::cppFilesString() const
{
	return mCppFiles;
}

QString CommonGeneratorInfo::includeFilesPlugin() const
{
	return mIncludeFilesPluginH;
}

QString CommonGeneratorInfo::privateFieldsPlugin() const
{
	return mPrivateFieldsPluginH;
}

QString CommonGeneratorInfo::ifForMainCheckPluginCpp() const
{
	return mIfForMainCheckPluginCPP;
}

QString CommonGeneratorInfo::replaceLanguageName(QString string
		, const Id &diagram
		, const int count)
{
//	string.replace("@@languageName@@", correctedLanguageName(diagram))
//			.replace("@@id@@", QString::number(count));
	return string;
}

