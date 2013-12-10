#include "generator.h"
#include "../../../qrutils/nameNormalizer.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QMessageBox>

using namespace game::generator;
using namespace qReal;
using namespace utils;

QString const templateDir = "./templates";
QString const templateFileNameTest = "testFile.cs";

/// Generation target file

Generator::Generator(QString const &outputDirPath
		, QString const &programName
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::GraphicalModelAssistInterface const &graphicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
	: AbstractGenerator(templateDir, QString("C:\\Users\\efimw_000\\Documents\\study\\").replace("\\", "/"), logicalModel, errorReporter)
	, mGraphicalModel(graphicalModel)
	, mProgramName(programName)
{
	Q_UNUSED(outputDirPath);
}

Generator::~Generator()
{
}

void Generator::generate()
{

	initGeneratingFiles();

	generateMainSwitch();

	saveGeneratedFiles();
}

void Generator::initGeneratingFiles()
{
	loadTemplateFromFile(templateFileNameTest, mResultTestFile);
}

void Generator::saveGeneratedFiles()
{
	QString outputFileName = mProgramName + ".cs";

	saveOutputFile(outputFileName, mResultTestFile);
}

void Generator::generateMainSwitch()
{
	IdList toGenerate;
	toGenerate << mApi.elementsByType("Frame");

	QString outerSwitchBody = "";
	foreach (Id const &curFrame, toGenerate) {
		if (mApi.isLogicalElement(curFrame)) {
			continue;
		}

		QString currentFrameCase = mTemplateUtils["@@eachFrameSwitchBody@@"];

		QString currentFrameName = mApi.property(curFrame, "name").toString();

		QString innerSwitch = generateFrameRelatedSwitch(curFrame);


		currentFrameCase.replace("@@frameName@@", currentFrameName);
		currentFrameCase.replace("@@caseForConnectedFrame@@", innerSwitch);

	outerSwitchBody += currentFrameCase;
	}

	mResultTestFile.replace("@@eachFrameSwitches@@", outerSwitchBody);
}

QString Generator::generateScreenSwitchArrow(const Id &edge)
{
	Id diagram = mApi.LogicalRepoApi.outgoingExplosion(edge);
	IdList elements = mApi.LogicalRepoApi.children(diagram);
	Id start = Id();
	QString result = "";

	foreach (Id element, elements) {
		if (mApi.isLogicalElement(element) &&
				0 == mApi.incomingLinks(element).length())
			start = element;
	}
	if (Id() == start) {
		return ("	//The diagram of"
				+ mApi.property(edge, "TextProperty")
				+ "is not proper");
	}

	for (Id elem = start; mApi.outgoingLinks(elem).length() != 0; elem = mApi.outgoingLinks(elem).at(0)){
		QString lineTemplate = "";
		switch (elem.element()) {
		case "CodeArea" :
			lineTemplate = mTemplateUtils["@@CodeArea@@"];
			QString inlineCode = mApi.property(elem, "Code");
			lineTemplate.replace("@@inlineCode@@", inlineCode);
			break;
		case "EndState":
			lineTemplate = mTemplateUtils["@@EndState@@"];
			QString endStateName = mApi.property(elem, "StateName");
			lineTemplate.replace("@@StateName@@", endStateName);
			break;
		}
		result += lineTemplate;
	}


	return result;
}

QString Generator::generateFrameRelatedSwitch(Id const &currentFrame)
{
/*	QMessageBox msgBox;
	msgBox.setText("The document has been modified.");
	msgBox.setInformativeText("Do you want to save your changes?");
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Save);
	int ret = msgBox.exec();
*/

	QString frameSwitch = "";

	IdList edges = mApi.elementsByType("ScreenTransition");

	foreach (Id const &edge, edges) {
		if (mApi.isLogicalElement(edge)) {
			continue;
		}



		//IdList const &graphicalEdges = mGraphicalModel.graphicalIdsByLogicalId(edge);
		Id from = mGraphicalModel.from(edge);

		if (from != currentFrame) {
			from = mGraphicalModel.graphicalRepoApi().parent(from);
			if (from != currentFrame)
				continue;
		}

		QString linkCase = mTemplateUtils["@@outconnectedFrameCase@@"];

		Id rightHandFrame = mGraphicalModel.to(edge);

		QString rightHandFrameName = mApi.property(rightHandFrame, "name").toString();

		linkCase.replace("@@rightHandFrameName@@", rightHandFrameName);
		frameSwitch += linkCase;
	}

	return frameSwitch;
}


