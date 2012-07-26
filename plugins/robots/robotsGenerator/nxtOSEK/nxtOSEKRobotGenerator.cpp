#include <QtCore/QTextStream>
#include <cmath>
#include <QtCore/QObject>
#include <QtCore/QDir>

#include "../../../../qrkernel/exception/exception.h"
#include "../../../../qrkernel/settingsManager.h"
#include "../../../../qrutils/inFile.h"
#include "../../../../qrutils/outFile.h"

#include "nxtOSEKRobotGenerator.h"
#include "elementGeneratorFactory.h"

using namespace qReal;
using namespace robots::generator;

NxtOSEKRobotGenerator::NxtOSEKRobotGenerator(
		Id const &diagram
		, qrRepo::RepoControlInterface &api
		, qReal::ErrorReporterInterface &errorReporter
		, QString const &destinationPath
		)
		: mDestinationPath(destinationPath)
		, mErrorReporter(errorReporter)
		, mDiagram(diagram)
{
	mIsNeedToDeleteMApi = false;
	mApi = dynamic_cast<qrRepo::RepoApi *>(&api);  // TODO: remove unneeded dynamic_cast or provide strong argumentation why it is needed.
}

NxtOSEKRobotGenerator::NxtOSEKRobotGenerator(QString const &pathToRepo
		, qReal::ErrorReporterInterface &errorReporter
		, QString const &destinationPath
		)
		: mDestinationPath(SettingsManager::value("temp", "").toString())
		, mErrorReporter(errorReporter)
{
	Q_UNUSED(destinationPath)
	mIsNeedToDeleteMApi = true;
	mApi = new qrRepo::RepoApi(pathToRepo);
}

NxtOSEKRobotGenerator::~NxtOSEKRobotGenerator()
{
	if (mApi && mIsNeedToDeleteMApi) {
		delete mApi;
	}
}

QString NxtOSEKRobotGenerator::generateVariableString()
{
	QString res;
	foreach (SmartLine const &curVariable, mVariables) {
		if (!curVariable.text().contains(" ")) {
			res = res + "static int " + curVariable.text() + ";\n";
		}
	}
	return "\n" + res;
}

QString NxtOSEKRobotGenerator::addTabAndEndOfLine(QList<SmartLine> const &lineList, QString resultCode)
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

void NxtOSEKRobotGenerator::generateMakeFile(
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
		mErrorReporter.addError(QObject::tr("There is nothing to generate, diagram doesn't have Initial Node or Initial Block"));
	}
}

void NxtOSEKRobotGenerator::insertCode(
		QString const &resultCode
		, QString const &resultInitCode
		, QString const &resultTerminateCode
		, QString const &curInitialNodeNumber)
{
	if (mBalancerIsActivated) {
		mResultString.replace("@@BALANCER@@", "#include \"balancer.h\"");
	} else {
		mResultString.replace("@@BALANCER@@", "");
	}
	mResultString.replace("@@CODE@@", resultCode +"\n" + "@@CODE@@").replace("@@VARIABLES@@"
			, generateVariableString() + "\n" + "@@VARIABLES@@").replace("@@INITHOOKS@@"
			, resultInitCode).replace("@@TERMINATEHOOKS@@", resultTerminateCode);
	mTaskTemplate.replace("@@NUMBER@@", curInitialNodeNumber);
	mResultOil.replace("@@TASK@@", mTaskTemplate + "\n" + "@@TASK@@");
}

void NxtOSEKRobotGenerator::deleteResidualLabels(QString const &projectName)
{
	mResultOil.replace("@@TASK@@", "");
	mResultString.replace("@@VARIABLES@@", "").replace("@@CODE@@", "").replace("@@PROJECT_NAME@@", projectName);
}

void NxtOSEKRobotGenerator::generateFilesForBalancer(QString const &projectDir)
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

void NxtOSEKRobotGenerator::createProjectDir(QString const &projectDir)
{
	if (!QDir(projectDir).exists()) {
		if (!QDir("nxt-tools/").exists()) {
			QDir().mkdir("nxt-tools/");
		}
		QDir().mkdir(projectDir);
	}
}

void NxtOSEKRobotGenerator::generate()
{
	if (mDiagram == Id()) {
		mErrorReporter.addCritical(QObject::tr("There is no opened diagram"));
		return;
	}

	IdList toGenerate;
	toGenerate << mApi->elementsByType("InitialNode");
	toGenerate << mApi->elementsByType("InitialBlock");

	int curInitialNodeNumber = 0;
	//QDir projectsDir; //TODO: use user path to projects
	QString const projectName = "example" + QString::number(curInitialNodeNumber);
	QString const projectDir = "nxt-tools/" + projectName;
	createProjectDir(projectDir);

	mResultString = utils::InFile::readAll(":/nxtOSEK/templates/template.c");
	mResultOil = utils::InFile::readAll(":/nxtOSEK/templates/template.oil");

	QString resultTaskTemplate = utils::InFile::readAll(":/nxtOSEK/templates/taskTemplate.oil");

	foreach (Id const &curInitialNode, toGenerate) {
		if (!mApi->isGraphicalElement(curInitialNode)) {
			continue;
		}

		if (mApi->parent(curInitialNode) != mDiagram) {
			continue;
		}

		initializeFields(resultTaskTemplate, curInitialNode);

		AbstractElementGenerator* const gen = ElementGeneratorFactory::generator(this, curInitialNode, *mApi);
		gen->generate(); //may throws a exception
		delete gen;

		addResultCodeInCFile(curInitialNodeNumber);
		curInitialNodeNumber++;
	}

	outputInCAndOilFile(projectName, projectDir, toGenerate);
}

QList<SmartLine> &NxtOSEKRobotGenerator::variables()
{
	return mVariables;
}

QList<SmartLine> &NxtOSEKRobotGenerator::initCode()
{
	return mInitCode;
}

QList<SmartLine> &NxtOSEKRobotGenerator::terminateCode()
{
	return mTerminateCode;
}

qrRepo::RepoApi const * const NxtOSEKRobotGenerator::api() const
{
	return mApi;
}

QByteArray NxtOSEKRobotGenerator::portValue1() const
{
	return mPortValue1;
}

QByteArray NxtOSEKRobotGenerator::portValue2() const
{
	return mPortValue2;
}

QByteArray NxtOSEKRobotGenerator::portValue3() const
{
	return mPortValue3;
}

QByteArray NxtOSEKRobotGenerator::portValue4() const
{
	return mPortValue4;
}

void NxtOSEKRobotGenerator::setPortValue1(QByteArray portValue)
{
	mPortValue1 = portValue;
}

void NxtOSEKRobotGenerator::setPortValue2(QByteArray portValue)
{
	mPortValue2 = portValue;
}

void NxtOSEKRobotGenerator::setPortValue3(QByteArray portValue)
{
	mPortValue3 = portValue;
}

void NxtOSEKRobotGenerator::setPortValue4(QByteArray portValue)
{
	mPortValue4 = portValue;
}
ErrorReporterInterface &NxtOSEKRobotGenerator::errorReporter()
{
	return mErrorReporter;
}

qReal::Id &NxtOSEKRobotGenerator::previousElement()
{
	return mPreviousElement;
}

QList<QList<SmartLine> > &NxtOSEKRobotGenerator::generatedStringSet()
{
	return mGeneratedStringSet;
}

void NxtOSEKRobotGenerator::setGeneratedStringSet(int key, QList<SmartLine> const &list)
{
	mGeneratedStringSet[key] = list;
}

QMap<QString, QStack<int> > &NxtOSEKRobotGenerator::elementToStringListNumbers()
{
	return mElementToStringListNumbers;
}

int NxtOSEKRobotGenerator::elementToStringListNumbersPop(QString const &key)
{
	return mElementToStringListNumbers[key].pop();
}

QStack<qReal::Id> &NxtOSEKRobotGenerator::previousLoopElements()
{
	return mPreviousLoopElements;
}

qReal::Id NxtOSEKRobotGenerator::previousLoopElementsPop()
{
	return mPreviousLoopElements.pop();
}

void NxtOSEKRobotGenerator::addResultCodeInCFile(int curInitialNodeNumber)
{
	QString resultCode;
	mCurTabNumber = 0;
	foreach (QList<SmartLine> const &lineList, mGeneratedStringSet) {
		 resultCode = addTabAndEndOfLine(lineList, resultCode);
	}

	QString resultInitCode;
	resultInitCode = addTabAndEndOfLine(mInitCode, resultInitCode);
	QString resultTerminateCode;
	resultTerminateCode = addTabAndEndOfLine(mTerminateCode, resultTerminateCode);
	resultCode = "TASK(OSEK_Task_Number_" + QString::number(curInitialNodeNumber) +")\n{\n" + resultCode + "}";
	insertCode(resultCode, resultInitCode, resultTerminateCode, QString::number(curInitialNodeNumber));
}

void NxtOSEKRobotGenerator::outputInCAndOilFile(QString const projectName, QString const projectDir
		,IdList toGenerate)
{
	deleteResidualLabels(projectName);
	//Output in the .c and .oil file
	utils::OutFile outC(projectDir + "/" + projectName + ".c");
	outC() << mResultString;
	utils::OutFile outOil(projectDir + "/" + projectName + ".oil");
	outOil() << mResultOil;
	generateFilesForBalancer(projectDir);
	generateMakeFile(toGenerate.isEmpty(), projectName, projectDir);
}

void NxtOSEKRobotGenerator::initializeFields(QString resultTaskTemplate, Id const curInitialNode)
{
	mTaskTemplate = resultTaskTemplate;
	mGeneratedStringSet.clear();
	mGeneratedStringSet.append(QList<SmartLine>()); //first list for variable initialization
	mVariablePlaceInGenStrSet = 0;
	mElementToStringListNumbers.clear();
	mVariables.clear();
	mPreviousElement = curInitialNode;
	mBalancerIsActivated = false;
}
