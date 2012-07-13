#include "nxtOSEKRobotGenerator.h"

#include <QTextStream>
#include <cmath>
#include <QtCore/QObject>
#include <QDir>

#include "../../../../qrkernel/exception/exception.h"
#include "../../../../qrkernel/settingsManager.h"

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
{
	mIsNeedToDeleteMApi = false;
	mApi = dynamic_cast<qrRepo::RepoApi *>(&api);  // TODO: remove unneeded dynamic_cast or provide strong argumentation why it is needed.
	mDiagram = diagram;
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
	if (mApi && mIsNeedToDeleteMApi)
		delete mApi;
}

void NxtOSEKRobotGenerator::addToGeneratedStringSetVariableInit()
{
	/*foreach (SmartLine curVariable, mVariables) {
		mGeneratedStringSet[mVariablePlaceInGenStrSet].append(
				SmartLine("int " + curVariable.text() + ";", curVariable.elementId()));
	}*/
}

QString NxtOSEKRobotGenerator::generateVariableString()
{
	QString res;
	foreach (SmartLine curVariable, mVariables) {
		if (!curVariable.text().contains(" ")) {
			res = res + "static int " + curVariable.text() + ";\n";
		}
	}
	return res;
}

QString NxtOSEKRobotGenerator::addTabAndEndOfLine(QList<SmartLine> const &lineList, QString resultCode)
{
	foreach (SmartLine const &curLine, lineList) {
		if ( (curLine.indentLevelChange() == SmartLine::decrease)
			|| (curLine.indentLevelChange() == SmartLine::increaseDecrease) )
		{
			mCurTabNumber--;
		}
		resultCode += QString(mCurTabNumber, '\t') + curLine.text() + "\n";
		if ( (curLine.indentLevelChange() == SmartLine::increase)
			|| (curLine.indentLevelChange() == SmartLine::increaseDecrease) )
		{
			mCurTabNumber++;
		}
	}
	return resultCode;
}

void NxtOSEKRobotGenerator::generateMakeFile(bool toGenerateIsEmpty, QString projectName, QString projectDir)
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
	if (mBalancerIsActivated)
		outMake << templateMakeFile.readAll().replace("@@PROJECT_NAME@@", projectName.toUtf8()).replace("@@BALANCER@@", "balancer_param.c \\").replace("@@BALANCER_LIB@@", "USER_LIB = nxtway_gs_balancer");
	else
		outMake << templateMakeFile.readAll().replace("@@PROJECT_NAME@@", projectName.toUtf8()).replace("@@BALANCER@@", "").replace("@@BALANCER_LIB@@", "");
	templateMakeFile.close();

	outMake.flush();
	resultMakeFile.close();

	if (toGenerateIsEmpty) {
		mErrorReporter.addError(QObject::tr("There is nothing to generate, diagram doesn't have Initial Node or Initial Block"));
	}

}

void NxtOSEKRobotGenerator::insertCode(
		QString resultCode,
		QString resultInitCode,
		QString resultTerminateCode,
		QString curInitialNodeNumber)
{
	if (mBalancerIsActivated) {
		mResultString.replace("@@BALANCER@@", "#include \"balancer.h\"");
	} else {
		mResultString.replace("@@BALANCER@@", "");
	}
	mResultString.replace("@@CODE@@", resultCode +"\n" + "@@CODE@@");
	mTaskTemplate.replace("@@NUMBER@@", curInitialNodeNumber);
	mResultOIL.replace("@@TASK@@", mTaskTemplate + "\n" + "@@TASK@@");
	mResultString.replace("@@VARIABLES@@", generateVariableString() + "\n" + "@@VARIABLES@@");
	mResultString.replace("@@INITHOOKS@@", resultInitCode);
	mResultString.replace("@@TERMINATEHOOKS@@", resultTerminateCode);
}

void NxtOSEKRobotGenerator::deleteResidualLabels(QString projectName)
{
	mResultOIL.replace("@@TASK@@", "");
	mResultString.replace("@@VARIABLES@@", "");
	mResultString.replace("@@CODE@@", "");
	mResultString.replace("@@PROJECT_NAME@@", projectName);
}

void NxtOSEKRobotGenerator::generateFilesForBalancer(QString projectDir)
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

void NxtOSEKRobotGenerator::generate()
{
	if (mDiagram == Id()) {
		mErrorReporter.addCritical(QObject::tr("There is no opened diagram"));
		return;
	}

	IdList toGenerate = mApi->elementsByType("InitialNode");
	toGenerate << mApi->elementsByType("InitialBlock");

	int curInitialNodeNumber = 0;

	QString resultInitCode;
	QString resultTerminateCode;

	//QDir projectsDir; //TODO: use user path to projects
	QString projectName = "example" + QString::number(curInitialNodeNumber);
	QString projectDir = "nxt-tools/" + projectName;

	//Create project directory
	if (!QDir(projectDir).exists()) {
		if (!QDir("nxt-tools/").exists())
			QDir().mkdir("nxt-tools/");
		QDir().mkdir(projectDir);
	}

	// Template C file
	QFile templateCFile(":/nxtOSEK/templates/template.c");
	if (!templateCFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + templateCFile.fileName() + "\"");
		return;
	}

	QTextStream templateCStream(&templateCFile);
	mResultString = templateCStream.readAll();
	templateCFile.close();

	// Generate OIL file
	QFile resultOILFile(projectDir + "/" + projectName + ".oil");
	if (!resultOILFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + resultOILFile.fileName() + "\"");
		return;
	}

	// Template OIL file
	QFile templateOILFile(":/nxtOSEK/templates/template.oil");
	if (!templateOILFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + templateOILFile.fileName() + "\"");
		return;
	}

	mResultOIL = templateOILFile.readAll();
	templateOILFile.close();

	// Task template file
	QFile taskTemplateFile(":/nxtOSEK/templates/taskTemplate.oil");
	if (!taskTemplateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + taskTemplateFile.fileName() + "\"");
		return;
	}

	QString resultTaskTemplate = taskTemplateFile.readAll();
	taskTemplateFile.close();

	foreach (Id const &curInitialNode, toGenerate) {

		mTaskTemplate = resultTaskTemplate;
		if (!mApi->isGraphicalElement(curInitialNode)) {
			continue;
		}

		if (mApi->parent(curInitialNode) != mDiagram) {
			continue;
		}

		mGeneratedStringSet.clear();
		mGeneratedStringSet.append(QList<SmartLine>()); //first list for variable initialization
		mVariablePlaceInGenStrSet = 0;

		mElementToStringListNumbers.clear();
		mVariables.clear();

		AbstractElementGenerator* gen = ElementGeneratorFactory::generator(this, curInitialNode, *mApi);
		mPreviousElement = curInitialNode;

		mBalancerIsActivated = false;
		gen->generate(); //may throws a exception
		delete gen;
		addToGeneratedStringSetVariableInit();

		// Result code in .c file
		QString resultCode;
		mCurTabNumber = 0;
		foreach (QList<SmartLine> lineList, mGeneratedStringSet) {
			 resultCode = addTabAndEndOfLine(lineList, resultCode);
		}
		// Code init block in .c file
		resultInitCode = addTabAndEndOfLine(mInitCode, resultInitCode);
		// Code terminate block in .c file
		resultTerminateCode = addTabAndEndOfLine(mTerminateCode, resultTerminateCode);
		resultCode = "TASK(OSEK_Task_Number_" + QString::number(curInitialNodeNumber) +")\n{\n" + resultCode + "}";
		insertCode(resultCode, resultInitCode, resultTerminateCode, QString::number(curInitialNodeNumber));
		curInitialNodeNumber++;
	}
	deleteResidualLabels(projectName);

	QFile resultCFile(projectDir + "/" + projectName + ".c");
	if (!resultCFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + resultCFile.fileName() + "\"");
		return;
	}

	//Output in the .c and .oil file
	QTextStream outC(&resultCFile);
	outC << mResultString;
	outC.flush();
	resultCFile.close();

	QTextStream outOIL(&resultOILFile);
	outOIL << mResultOIL;
	outOIL.flush();
	resultOILFile.close();

	generateFilesForBalancer(projectDir);
	generateMakeFile(toGenerate.isEmpty(), projectName, projectDir);
}

QList<SmartLine> &NxtOSEKRobotGenerator::variables()
{
	return mVariables;
}

QList<SmartLine> &NxtOSEKRobotGenerator::initCode()
{
	return mInitCode;
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

ErrorReporterInterface &NxtOSEKRobotGenerator::errorReporter() const
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

QMap<QString, QStack<int> > &NxtOSEKRobotGenerator::elementToStringListNumbers()
{
	return mElementToStringListNumbers;
}

QStack<qReal::Id> &NxtOSEKRobotGenerator::previousLoopElements()
{
	return mPreviousLoopElements;
}
