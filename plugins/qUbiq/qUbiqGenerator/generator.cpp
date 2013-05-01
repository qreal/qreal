#include "generator.h"
#include "../../../qrutils/nameNormalizer.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace qUbiq::qUbiqGenerator;
using namespace qReal;
using namespace utils;

QString const templateDir = "./templates";
QString const fileNameForms = "qUbiqForms.cs";
QString const fileNameVariables = "qUbiqVariables.cs";
QString const fileNameCSProj = "qUbiqCSProject.cs";

/// Generation target file

Generator::Generator(QString const &outputDirPath
		, QString const &pathToQReal
		, QString const &programName
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
	: AbstractGenerator(templateDir, outputDirPath + "/" + programName, logicalModel, errorReporter)
	, mPathToQReal(pathToQReal + "/.."), mProgramName(programName)
{
	mPathToQReal.replace("\\", "/");
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

void Generator::initGeneratedStrings()
{
	loadTemplateFromFile(fileNameForms, mResultForms);
	loadTemplateFromFile(fileNameVariables, mResultVariables);
	loadTemplateFromFile(fileNameCSProj, mResultCSProject);
}

void Generator::generate()
{
	mErrorReporter.clear();
	mErrorReporter.clearErrors();

	if (!isCorrectedName(mProgramName)) {
		mErrorReporter.addCritical("Program name is not correct");
		return;
	}

	initGeneratedStrings();
	generatePresentationDiagrams();
	generateLogicDiagrams();
	generateCSProject();
}

void Generator::generatePresentationDiagrams()
{
	foreach (Id const &diagram, mApi.elementsByType("qUbiqPresentationDiagram")) {
		if (!mApi.isLogicalElement(diagram) || mApi.parent(diagram) != Id::rootId()) {
			continue;
		}
		generateMainForms(diagram);
		generateVariables(diagram);
	}
}

void Generator::generateLogicDiagrams()
{
	QList<Id> logicElementList= mApi.elementsByType("qUbiqLogicDiagram");
	logicElementList.append(mApi.elementsByType("qUbiqConditionDiagram"));
	foreach (Id const &diagram, logicElementList) {
		if (!mApi.isLogicalElement(diagram) || mApi.parent(diagram) != Id::rootId()) {
			continue;
		}
		generateVariables(diagram);
		generateHandlers(diagram);
	}
}

QString Generator::generateVariables(Id const &diagram)
{
}

QString Generator::generateMainForms(Id const &diagram)
{
}

QString Generator::generateHandlers(Id const &diagram)
{
	Q_UNUSED(diagram)
	//qwerty_for future
}

void Generator::generateCSProject()
{
	//qwerty_without handlers
}

