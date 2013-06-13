#include "trikRobotGenerator.h"

#include <QtCore/QTextStream>
#include <cmath>
#include <QtCore/QObject>
#include <QtCore/QDir>

#include "../../../../qrkernel/exception/exception.h"
#include "../../../../qrkernel/settingsManager.h"
#include "../../../../qrutils/inFile.h"
#include "../../../../qrutils/outFile.h"

#include "elementGeneratorFactory.h"

using namespace qReal;
using namespace robots::trikGenerator;

TrikRobotGenerator::TrikRobotGenerator(
		Id const &diagram
		, qrRepo::RepoControlInterface &api
		, qReal::ErrorReporterInterface &errorReporter
		)
		: mErrorReporter(errorReporter)
		, mDiagram(diagram)
{
	mApi = dynamic_cast<qrRepo::RepoApi *>(&api);  // TODO: remove unneeded dynamic_cast or provide strong argumentation why it is needed.
}

TrikRobotGenerator::~TrikRobotGenerator()
{
}

QString TrikRobotGenerator::addTabAndEndOfLine(QList<SmartLine> const &lineList, QString resultCode)
{
	foreach (SmartLine const &curLine, lineList) {
		if (curLine.indentLevelChange() == SmartLine::decrease
			|| curLine.indentLevelChange() == SmartLine::decreaseOnlyThisLine)
		{
			mCurTabNumber--;
		}
		resultCode += QString(mCurTabNumber, '\t') + curLine.text() + "\n";
		if (curLine.indentLevelChange() == SmartLine::increase
			|| curLine.indentLevelChange() == SmartLine::decreaseOnlyThisLine)
		{
			mCurTabNumber++;
		}
	}
	return resultCode;
}

void TrikRobotGenerator::insertCode(QString const &resultCode)
{
	mResultString
			.replace("@@CODE@@", resultCode +"\n" + "@@CODE@@")
			.replace("@@VARIABLES@@", mVariables.generateVariableString() + "\n" + "@@VARIABLES@@")
			;
}

void TrikRobotGenerator::deleteResidualLabels()
{
	mResultString.replace("@@VARIABLES@@", "").replace("@@CODE@@", "");
}

void TrikRobotGenerator::generate(QString const &programName)
{
	if (mDiagram == Id()) {
		mErrorReporter.addCritical(QObject::tr("There is no opened diagram"));
		return;
	}

	IdList toGenerate(mApi->elementsByType("InitialNode"));

	int curInitialNodeNumber = 0;
	QString const projectName = programName;
	QString const projectDir = ".";

	initializeGeneration();

	bool generationOccured = false;
	foreach (Id const &curInitialNode, toGenerate) {
		if (!mApi->isGraphicalElement(curInitialNode)) {
			continue;
		}

		if (mApi->parent(curInitialNode) != mDiagram) {
			continue;
		}

		generationOccured = true;

		initializeFields(curInitialNode);

		AbstractElementGenerator* const gen = ElementGeneratorFactory::generator(this, curInitialNode, *mApi);
		gen->generate();
		delete gen;

		addResultCode();
		curInitialNodeNumber++;
	}

	if (!generationOccured) {
		mErrorReporter.addError(QObject::tr("There is nothing to generate, diagram doesn't have Initial Node"));
		return;
	}

	if (curInitialNodeNumber > 1) {
		mErrorReporter.addError(QObject::tr("Too many initial nodes, generation aborted"));
		return;
	}

	outputQtsFile(projectName);
}

void TrikRobotGenerator::initializeGeneration()
{
	mResultString = "@@VARIABLES@@\n@@CODE@@";
}

Variables &TrikRobotGenerator::variables()
{
	return mVariables;
}

//QList<SmartLine> &TrikRobotGenerator::initCode()
//{
//	return mInitCode;
//}

//QList<SmartLine> &TrikRobotGenerator::terminateCode()
//{
//	return mTerminateCode;
//}

//QList<SmartLine> &TrikRobotGenerator::isrHooksCode()
//{
//	return mIsrHooksCode;
//}

qrRepo::RepoApi const *TrikRobotGenerator::api() const
{
	return mApi;
}

interpreters::robots::sensorType::SensorTypeEnum TrikRobotGenerator::portValue(int port) const
{
	return static_cast<interpreters::robots::sensorType::SensorTypeEnum>(SettingsManager::value("port" + QString::number(port) + "SensorType").toInt());
}

ErrorReporterInterface &TrikRobotGenerator::errorReporter()
{
	return mErrorReporter;
}

qReal::Id &TrikRobotGenerator::previousElement()
{
	return mPreviousElement;
}

QList<QList<SmartLine> > &TrikRobotGenerator::generatedStringSet()
{
	return mGeneratedStringSet;
}

void TrikRobotGenerator::setGeneratedStringSet(int key, QList<SmartLine> const &list)
{
	mGeneratedStringSet[key] = list;
}

QString TrikRobotGenerator::intExpression(Id const &id, QString const &propertyName) const
{
	QString const expression = mApi->stringProperty(id, propertyName);
	return mVariables.expressionToInt(expression);
}

QMap<QString, QStack<int> > &TrikRobotGenerator::elementToStringListNumbers()
{
	return mElementToStringListNumbers;
}

int TrikRobotGenerator::elementToStringListNumbersPop(QString const &key)
{
	return mElementToStringListNumbers[key].pop();
}

QStack<qReal::Id> &TrikRobotGenerator::previousLoopElements()
{
	return mPreviousLoopElements;
}

qReal::Id TrikRobotGenerator::previousLoopElementsPop()
{
	return mPreviousLoopElements.pop();
}

void TrikRobotGenerator::addResultCode()
{
	QString resultCode;
	mCurTabNumber = 0;
	foreach (QList<SmartLine> const &lineList, mGeneratedStringSet) {
		 resultCode = addTabAndEndOfLine(lineList, resultCode);
	}

	insertCode(resultCode);
}

void TrikRobotGenerator::outputQtsFile(QString const fileName)
{
	deleteResidualLabels();
	utils::OutFile outC(fileName);
	outC() << mResultString;
}

void TrikRobotGenerator::initializeFields(Id const curInitialNode)
{
	mGeneratedStringSet.clear();
	mGeneratedStringSet.append(QList<SmartLine>());
	mElementToStringListNumbers.clear();
	mVariables.reinit(mApi);
	mPreviousElement = curInitialNode;
}
