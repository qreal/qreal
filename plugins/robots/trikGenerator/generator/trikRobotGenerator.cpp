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
		, QString const &destinationPath
		)
		: mDestinationPath(destinationPath)
		, mErrorReporter(errorReporter)
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
		resultCode += '\t' + QString(mCurTabNumber, '\t') + curLine.text() + "\n";
		if (curLine.indentLevelChange() == SmartLine::increase
			|| curLine.indentLevelChange() == SmartLine::decreaseOnlyThisLine)
		{
			mCurTabNumber++;
		}
	}
	return resultCode;
}

/*
void TrikRobotGenerator::generateMakeFile(
		bool const &toGenerateIsEmpty
		, QString const &projectName
		, QString const &projectDir)
{
	QFile templateMakeFile(":/nxtOSEK/templates/template.makefile");
	if (!templateMakeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + templateMakeFile.fileName() + "\"");
		return;
	}

	QFile resultMakeFile(projectDir + "/makefile");
	if (!resultMakeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + resultMakeFile.fileName() + "\"");
		return;
	}

	QTextStream outMake(&resultMakeFile);
	if (mBalancerIsActivated) {
		outMake << templateMakeFile.readAll().replace("@@PROJECT_NAME@@", projectName.toUtf8()).replace("@@BALANCER@@"
				, "balancer_param.c \\").replace("@@BALANCER_LIB@@", "USER_LIB = nxtway_gs_balancer");
	} else {
		outMake << templateMakeFile.readAll().replace("@@PROJECT_NAME@@", projectName.toUtf8()).replace("@@BALANCER@@"
				, "").replace("@@BALANCER_LIB@@", "");
		templateMakeFile.close();
	}

	outMake.flush();
	resultMakeFile.close();

	if (toGenerateIsEmpty) {
		mErrorReporter.addError(QObject::tr("There is nothing to generate, diagram doesn't have Initial Node"));
	}
}
*/

void TrikRobotGenerator::insertCode(
		QString const &resultCode
		, QString const &resultInitCode
		, QString const &resultTerminateCode
		, QString const &resultIsrHooksCode
		, QString const &curInitialNodeNumber)
{
//	if (mBalancerIsActivated) {
//		mResultString.replace("@@BALANCER@@", "#include \"balancer.h\"");
//	} else {
//		mResultString.replace("@@BALANCER@@", "");
//	}
//	mResultString.replace("@@CODE@@", resultCode +"\n" + "@@CODE@@").replace("@@VARIABLES@@"
//			, mVariables.generateVariableString() + "\n" + "@@VARIABLES@@").replace("@@INITHOOKS@@"
//			, resultInitCode).replace("@@TERMINATEHOOKS@@", resultTerminateCode)
//			.replace("@@USERISRHOOKS@@", resultIsrHooksCode);

	mResultString.replace("@@CODE@@", resultCode +"\n" + "@@CODE@@")
			.replace("@@VARIABLES@@", mVariables.generateVariableString() + "\n" + "@@VARIABLES@@")
			;

//	mTaskTemplate.replace("@@NUMBER@@", curInitialNodeNumber);
//	mResultOil.replace("@@TASK@@", mTaskTemplate + "\n" + "@@TASK@@");
}

void TrikRobotGenerator::deleteResidualLabels(QString const &projectName)
{
//	mResultOil.replace("@@TASK@@", "");
	mResultString.replace("@@VARIABLES@@", "").replace("@@CODE@@", "").replace("@@PROJECT_NAME@@", projectName);
}

/*
void TrikRobotGenerator::generateFilesForBalancer(QString const &projectDir)
{
	if (mBalancerIsActivated) {
		QFile::copy(":/nxtOSEK/templates/balancer/balancer_param.c", projectDir + "/" + "balancer_param.c");
		QFile::copy(":/nxtOSEK/templates/balancer/balancer.h", projectDir + "/" +"balancer.h");
		QFile::copy(":/nxtOSEK/templates/balancer/balancer_types.h", projectDir + "/" + "balancer_types.h");
		QFile::copy(":/nxtOSEK/templates/balancer/libnxtway_gs_balancer.a", projectDir + "/" + "libnxtway_gs_balancer.a");
		QFile::copy(":/nxtOSEK/templates/balancer/rt_SATURATE.h", projectDir + "/" + "rt_SATURATE.h");
		QFile::copy(":/nxtOSEK/templates/balancer/rtwtypes.h", projectDir + "/" + "rtwtypes.h");
	}
}

void TrikRobotGenerator::createProjectDir(QString const &projectDir)
{
	if (!QDir(projectDir).exists()) {
		if (!QDir("nxt-tools/").exists()) {
			QDir().mkdir("nxt-tools/");
		}
		QDir().mkdir(projectDir);
	}
}
*/

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

	initializeGeneration(projectDir);

//	QString resultTaskTemplate = utils::InFile::readAll(":/templates/taskTemplate.oil");

	bool generationOccured = false;
	foreach (Id const &curInitialNode, toGenerate) {
		if (!mApi->isGraphicalElement(curInitialNode)) {
			continue;
		}
		if (mApi->parent(curInitialNode) != mDiagram) {
			continue;
		}

		generationOccured = true;

		initializeFields(/*resultTaskTemplate, */curInitialNode);

		AbstractElementGenerator* const gen = ElementGeneratorFactory::generator(this, curInitialNode, *mApi);
		gen->generate(); //may throws a exception
		delete gen;

		addResultCodeInCFile(curInitialNodeNumber);
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

//	utils::OutFile file(programName);
//	file() << "print(\"Kill all humans!\")" << endl;
//	file() << "brick.motor(1).setPower(100)" << endl;
//	file() << "brick.motor(2).setPower(100)" << endl;

	outputInCAndOilFile(projectName, projectDir, toGenerate);
}

void TrikRobotGenerator::initializeGeneration(QString const &projectDir)
{
//	createProjectDir(projectDir);

//	mResultString = utils::InFile::readAll(":/nxtOSEK/templates/template.c");
	mResultString = "@@VARIABLES@@\n@@CODE@@";
//	mResultOil = utils::InFile::readAll(":/nxtOSEK/templates/template.oil");
}

Variables &TrikRobotGenerator::variables()
{
	return mVariables;
}

QList<SmartLine> &TrikRobotGenerator::initCode()
{
	return mInitCode;
}

QList<SmartLine> &TrikRobotGenerator::terminateCode()
{
	return mTerminateCode;
}

QList<SmartLine> &TrikRobotGenerator::isrHooksCode()
{
	return mIsrHooksCode;
}

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

void TrikRobotGenerator::addResultCodeInCFile(int curInitialNodeNumber)
{
	QString resultCode;
	mCurTabNumber = 0;
	foreach (QList<SmartLine> const &lineList, mGeneratedStringSet) {
		 resultCode = addTabAndEndOfLine(lineList, resultCode);
	}

	QString resultInitCode;
//	resultInitCode = addTabAndEndOfLine(mInitCode, resultInitCode);
	QString resultTerminateCode;
//	resultTerminateCode = addTabAndEndOfLine(mTerminateCode, resultTerminateCode);
	QString resultIsrHooksCode;
//	resultIsrHooksCode = addTabAndEndOfLine(mIsrHooksCode, resultIsrHooksCode);
//	resultCode = "TASK(OSEK_Task_Number_" + QString::number(curInitialNodeNumber) +")\n{\n" + resultCode + "}";
	insertCode(resultCode, resultInitCode, resultTerminateCode, resultIsrHooksCode, QString::number(curInitialNodeNumber));
}

void TrikRobotGenerator::outputInCAndOilFile(QString const projectName, QString const projectDir
		,IdList toGenerate)
{
	deleteResidualLabels(projectName);
	//Output in the .c and .oil file
	utils::OutFile outC(projectDir + "/" + projectName);
	outC() << mResultString;
//	utils::OutFile outOil(projectDir + "/" + projectName + ".oil");
//	outOil() << mResultOil;
//	generateFilesForBalancer(projectDir);
//	generateMakeFile(toGenerate.isEmpty(), projectName, projectDir);
}

void TrikRobotGenerator::initializeFields(/*QString resultTaskTemplate,*/ Id const curInitialNode)
{
//	mTaskTemplate = resultTaskTemplate;
	mGeneratedStringSet.clear();
	mGeneratedStringSet.append(QList<SmartLine>()); //first list for variable initialization
//	mVariablePlaceInGenStrSet = 0;
	mElementToStringListNumbers.clear();
	mVariables.reinit(mApi);
	mPreviousElement = curInitialNode;
//	mBalancerIsActivated = false;
}
