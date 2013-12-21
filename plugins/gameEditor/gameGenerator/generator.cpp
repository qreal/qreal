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

		QString edgeName = mApi.property(edge, "name").toString();

		QString edgeCode = generateScreenSwitchArrow(edge);

		QString rightHandFrameName = mApi.property(rightHandFrame, "name").toString();

		linkCase.replace("@@edgeName@@", edgeName);
		linkCase.replace("@@rightHandFrameName@@", rightHandFrameName);
		linkCase.replace("@@codeTemplate@@", edgeCode);
		frameSwitch += linkCase;
	}

	return frameSwitch;
}

QString Generator::generateScreenSwitchArrow(const Id &edge)
{
	Id logicalDiagram;
	if (mApi.isLogicalElement(edge)) {
		logicalDiagram = mApi.outgoingExplosion(edge);
	} else {
		logicalDiagram = mApi.outgoingExplosion(mGraphicalModel.logicalId(edge));
	}

	if (Id() == logicalDiagram) {
/*		return ("	//The diagram of"
				+ mApi.property(edge, "TextProperty")
				+ "does not exist");*/
		return "";
	}

	Id diagram = mGraphicalModel.graphicalIdsByLogicalId(logicalDiagram).at(0);


	IdList elements = mApi.children(diagram);
	Id start = Id();
	QString result = "";

	//put breakpoint here
	foreach (Id element, elements) {
		IdList inc = mApi.incomingLinks(element);
		IdList out = mApi.outgoingLinks(element);
	}

	foreach (Id element, elements) {
		if (0 == mApi.incomingLinks(element).length()
				&& element.type().element() != "edge"
				&& element.type().element() != "Edge")
			start = element;
	}



	Id elem = start;
	for ( ; ; ){
		QString lineTemplate = "";
		if (elem.element() == "CodeArea") {
			lineTemplate = mTemplateUtils["@@codeArea@@"];
			QString inlineCode = mApi.property(mGraphicalModel.logicalId(elem), "Code").toString();
			lineTemplate.replace("@@inlineCode@@", inlineCode);
		}
		if (elem.element() == "EndState") {
			lineTemplate = mTemplateUtils["@@endState@@"];
			QString endStateName = mApi.property(mGraphicalModel.logicalId(elem), "StateName").toString();
			lineTemplate.replace("@@stateName@@", endStateName);
		}
		result += lineTemplate;

		if (mApi.outgoingLinks(elem).length() == 0) {
			break;
		}

		IdList links = mApi.outgoingLinks(elem);
		//it will definetly will not generate IF block that way
		Id link = links.at(0);
		Id newElem = mGraphicalModel.graphicalRepoApi().otherEntityFromLink(link, elem);
		elem = newElem;
	}


	return result;
}
