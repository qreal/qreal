﻿#include <QtCore/QTextStream>
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
		: mDestinationPath(SettingsManager::value("temp").toString())
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
		, "balancer_param.c \\").replace("@@BALANCER_LIB@@", "USER_LIB = nxtway_gs_balancer").replace("@@BMPFILES@@"
		,  mImageGenerator.generateBmpFilesStringForMake().toUtf8());
	} else {
		outMake << templateMakeFile.readAll().replace("@@PROJECT_NAME@@", projectName.toUtf8()).replace("@@BALANCER@@"
		, "").replace("@@BALANCER_LIB@@", "").replace("@@BMPFILES@@"
		, mImageGenerator.generateBmpFilesStringForMake().toUtf8());;
		templateMakeFile.close();
	}

	outMake.flush();
	resultMakeFile.close();

	if (toGenerateIsEmpty) {
		mErrorReporter.addError(QObject::tr("There is nothing to generate, diagram doesn't have Initial Node"));
	}
}

void NxtOSEKRobotGenerator::insertCode(
		QString const &resultCode
		, QString const &resultInitCode
		, QString const &resultTerminateCode
		, QString const &resultIsrHooksCode
		, QString const &curInitialNodeNumber)
{
	if (mBalancerIsActivated) {
		mResultString.replace("@@BALANCER@@", "#include \"balancer.h\"");
	} else {
		mResultString.replace("@@BALANCER@@", "");
	}
	mResultString.replace("@@CODE@@", resultCode +"\n" + "@@CODE@@").replace("@@VARIABLES@@"
			, mVariables.generateVariableString() + "\n" + "@@VARIABLES@@").replace("@@INITHOOKS@@"
			, resultInitCode + "\n" + "@@INITHOOKS@@").replace("@@TERMINATEHOOKS@@", resultTerminateCode + "\n" + "@@TERMINATEHOOKS@@")
			.replace("@@USERISRHOOKS@@", resultIsrHooksCode).replace("@@BMPFILES@@"
			, mImageGenerator.generateBmpFilesStringForC() + "@@BMPFILES@@");

	mTaskTemplate.replace("@@NUMBER@@", curInitialNodeNumber);
	mResultOil.replace("@@TASK@@", mTaskTemplate + "\n" + "@@TASK@@");
	mResultString.replace("@@TASKDECLARATION@@", "DeclareTask(OSEK_Task_Number_0);\n@@TASKDECLARATION@@");
qDebug() << "1here!";
	mBtCommunicationGenerator.generateBtCommCode(mResultOil, mResultString, mTaskTemplate);
	qDebug() << "2here!";
	//BT Communication code
	//if (mIsBtMaster || mIsBtSlave) {
		//mResultOil.replace("PRIORITY = 1; /* lowest priority */", "PRIORITY = 2;\n\tEVENT = BtConnect;");
		//mResultOil.replace("@@TASK@@", mTaskTemplate.replace("OSEK_Task_Number_0", "IdleTask")
		//+ "\n" + "@@TASK@@");
		/*mResultOil.replace("@@EVENT@@", "EVENT BtConnect\n{\n\tMASK = AUTO;\n};");

		mResultString.replace("@@TASKDECLARATION@@", "DeclareTask(IdleTask);\n@@TASKDECLARATION@@");
		mResultString.replace("@@TASKDECLARATION@@", "DeclareEvent(BtConnect);\n@@TASKDECLARATION@@");
		mResultString.replace("@@DEFINES@@", "#define RUNTIME_CONNECTION\n@@DEFINES@@");
		mResultString.replace("@@TERMINATEHOOKS@@", "\tecrobot_term_bt_connection();");
		mResultString += "\nTASK(IdleTask)\n{\n\tstatic SINT bt_status = BT_NO_INIT;\n\twhile(1)\n\t{\n#ifdef RUNTIME_CONNECTION\n";
		if(mIsBtMaster) {
			mResultString.replace("@@INITHOOKS@@", "#ifndef RUNTIME_CONNECTION\n\tecrobot_init_bt_master(bd_addr, \"LEJOS-OSEK\");\n#endif");
			mResultString += "\t\tecrobot_init_bt_master(bd_addr, \"LEJOS-OSEK\");\n#endif\n";
		} else {
			mResultString.replace("@@INITHOOKS@@", "#ifndef RUNTIME_CONNECTION\n\tecrobot_init_bt_slave(\"LEJOS-OSEK\");\n#endif");
			mResultString += "\t\tecrobot_init_bt_slave(\"LEJOS-OSEK\");\n#endif\n";
		}
		mResultString += "\tif (ecrobot_get_bt_status() == BT_STREAM && bt_status != BT_STREAM)\n";
		mResultString += "\t{\n\t\tSetEvent(OSEK_Task_Number_0, BtConnect);\n\t}";
		mResultString += "\n\tbt_status = ecrobot_get_bt_status();\n\t}\n}";
	}*/
	mResultString.replace("@@INITHOOKS@@", "").replace("@@TERMINATEHOOKS@@", "");
}

void NxtOSEKRobotGenerator::deleteResidualLabels(QString const &projectName)
{
	mResultOil.replace("@@TASK@@", "");
	mResultString.replace("@@VARIABLES@@", "").replace("@@BMPFILES@@", "").replace("@@CODE@@"
		, "").replace("@@PROJECT_NAME@@", projectName).replace("@@DEFINES@@", "").replace("@@TASKDECLARATION@@", "");
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

	IdList toGenerate(mApi->elementsByType("InitialNode"));

	int curInitialNodeNumber = 0;
	QString const projectName = "example" + QString::number(curInitialNodeNumber);
	QString const projectDir = "nxt-tools/" + projectName;

	initializeGeneration(projectDir);

	QString resultTaskTemplate = utils::InFile::readAll(":/nxtOSEK/templates/taskTemplate.oil");

	bool generationOccured = false;
	foreach (Id const &curInitialNode, toGenerate) {
		if (!mApi->isGraphicalElement(curInitialNode)) {
			continue;
		}
		if (mApi->parent(curInitialNode) != mDiagram) {
			continue;
		}
		generationOccured = true;

		initializeFields(resultTaskTemplate, curInitialNode);

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
	outputInCAndOilFile(projectName, projectDir, toGenerate);
}

void NxtOSEKRobotGenerator::initializeGeneration(QString const &projectDir)
{
	createProjectDir(projectDir);

	mResultString = utils::InFile::readAll(":/nxtOSEK/templates/template.c");
	mResultOil = utils::InFile::readAll(":/nxtOSEK/templates/template.oil");
}

Variables &NxtOSEKRobotGenerator::variables()
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

QList<SmartLine> &NxtOSEKRobotGenerator::isrHooksCode()
{
	return mIsrHooksCode;
}

qrRepo::RepoApi const *NxtOSEKRobotGenerator::api() const
{
	return mApi;
}

interpreters::robots::sensorType::SensorTypeEnum NxtOSEKRobotGenerator::portValue(int port) const
{
	return static_cast<interpreters::robots::sensorType::SensorTypeEnum>(SettingsManager::value("port" + QString::number(port) + "SensorType").toInt());
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

QString NxtOSEKRobotGenerator::intExpression(Id const &id, QString const &propertyName) const
{
	QString const expression = mApi->stringProperty(id, propertyName);
	return mVariables.expressionToInt(expression);
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
	QString resultIsrHooksCode;
	resultIsrHooksCode = addTabAndEndOfLine(mIsrHooksCode, resultIsrHooksCode);
	resultCode = "TASK(OSEK_Task_Number_" + QString::number(curInitialNodeNumber) +")\n{\n" + resultCode + "}";
	insertCode(resultCode, resultInitCode, resultTerminateCode, resultIsrHooksCode, QString::number(curInitialNodeNumber));
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
	mVariables.reinit(mApi);
	mPreviousElement = curInitialNode;
	mBalancerIsActivated = false;
	mImageGenerator.reinit();
	mBtCommunicationGenerator.reinit();
}

ImageGenerator &NxtOSEKRobotGenerator::imageGenerator()
{
	return mImageGenerator;
}

BtCommunicationGenerator &NxtOSEKRobotGenerator::btCommunicationGenerator()
{
	return mBtCommunicationGenerator;
}
