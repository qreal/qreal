#include "generator.h"
#include "../../../qrutils/nameNormalizer.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace qUbiq::qUbiqGenerator;
using namespace qReal;
using namespace utils;

QString const templateDir = "./templates";
QString const templateFileNameForms = "qUbiqForms.cs";
QString const templateFileNameVariables = "qUbiqVariables.cs";
QString const templateFfileNameCSProj = "qUbiqCSProject.cs";

/// Generation target file

Generator::Generator(QString const &outputDirPath
		, QString const &programName
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
	: AbstractGenerator(templateDir, QString(outputDirPath + "/" + programName).replace("\\", "/"), logicalModel, errorReporter)
	, mProgramName(programName)
{
}

Generator::~Generator()
{
}

bool Generator::isCorrectedName(QString const &name)
{
	QRegExp patten;
	patten.setPattern("[A-Za-z]+([A-Za-z_0-9]*)");
	return patten.exactMatch(name);
}

void Generator::initGeneratingFiles()
{
	loadTemplateFromFile(templateFileNameForms, mResultForms);
	loadTemplateFromFile(templateFileNameVariables, mResultVariables);
}

void Generator::saveGeneratedFiles()
{
	QString formsFileName = QString(mProgramName + "Forms.cs");
	QString variablesFileName = QString(mProgramName + "Variables.cs");

	saveOutputFile(formsFileName, mResultForms);
	saveOutputFile(variablesFileName, mResultVariables);

	mCompileIncludeFiles.append(formsFileName);
	mCompileIncludeFiles.append(variablesFileName);
}

void Generator::generate()
{
	mErrorReporter.clear();
	mErrorReporter.clearErrors();

	if (!isCorrectedName(mProgramName)) {
		mErrorReporter.addCritical("Program name is not correct");
		return;
	}

	initGeneratingFiles();
	generateVariables();
	generatePresentationDiagrams();
	generateLogicDiagrams();
	saveGeneratedFiles();
	generateAndSaveCSProject();
}

void Generator::generatePresentationDiagrams()
{
	QString startFormName = "";
	QString formsDescription = "";
	QString onButtonClickedDescriptions = "";

	foreach (Id const &diagram, mApi.elementsByType("qUbiqPresentationDiagram")) {
		if (!mApi.isLogicalElement(diagram) || mApi.parent(diagram) != Id::rootId()) {
			continue;
		}
		NeededStringsForPresentationDiagram currentForms = generateMainForms(diagram);
		startFormName = currentForms.startFormName;
		formsDescription += currentForms.formsDescription;
		onButtonClickedDescriptions += currentForms.onButtonClickedDescriptions;
	}

	mResultForms.replace("@@programName@@", mProgramName);
	mResultForms.replace("@@startFormName@@", startFormName);
	mResultForms.replace("@@createFormDescriptions@@", formsDescription);
	mResultForms.replace("@@onButtonClickedDescriptions@@", onButtonClickedDescriptions);
}

void Generator::generateLogicDiagrams()
{
	QList<Id> logicElementList= mApi.elementsByType("qUbiqLogicDiagram");
	logicElementList.append(mApi.elementsByType("qUbiqConditionDiagram"));
	foreach (Id const &diagram, logicElementList) {
		if (!mApi.isLogicalElement(diagram) || mApi.parent(diagram) != Id::rootId()) {
			continue;
		}
		generateHandlers(diagram);
	}
}

void Generator::generateVariables()
{
	QString variablesDeclaration = "";

	foreach (Id const &variable, mApi.elementsByType("variable")) {
		if (!mApi.isLogicalElement(variable)) {
			continue;
		}
		QString currentVariable = mTemplateUtils["@@oneVariableDeclaration@@"];
		QString variableType = mApi.property(variable, "type").toString();

		QString realVariableType = "";
		if (variableType == "text") {
			realVariableType = "string";
		} else if (variableType == "list") {
			realVariableType = "List"; //qwerty_what
		} else if (variableType == "image") {
			realVariableType = "Image";
		} else if (variableType == "grid") {
			realVariableType = "Grid";
		} else if (variableType == "number") {
			realVariableType = "int";
		}

		currentVariable.replace("@@variableName@@", mApi.name(variable));
		currentVariable.replace("@@variableType@@", realVariableType);
		variablesDeclaration += currentVariable;
	}

	mResultVariables.replace("@@programName@@", mProgramName);
	mResultVariables.replace("@@variableDeclarations@@", variablesDeclaration);
}

Generator::NeededStringsForPresentationDiagram Generator::generateMainForms(Id const &diagram)
{
	return Generator::NeededStringsForPresentationDiagram("", "", "");
}

void Generator::generateHandlers(Id const &diagram)
{
	Q_UNUSED(diagram)
	//qwerty_TODO_for future
}

void Generator::generateAndSaveCSProject()
{
	QString resultCSProject;
	loadTemplateFromFile(templateFfileNameCSProj, resultCSProject);

	QString compileIncludes = "";
	foreach (QString fileName, mCompileIncludeFiles)
	{
		QString oneCompileIncludeTemplate = mTemplateUtils["@@oneCompileInclude@@"];
		oneCompileIncludeTemplate.replace("@@fileName@@", fileName);
		compileIncludes += oneCompileIncludeTemplate;
	}

	resultCSProject.replace("@@programName@@", mProgramName);
	resultCSProject.replace("@@compileIncludes@@", compileIncludes);

	saveOutputFile(QString(mProgramName + ".csproj"), resultCSProject);
}

