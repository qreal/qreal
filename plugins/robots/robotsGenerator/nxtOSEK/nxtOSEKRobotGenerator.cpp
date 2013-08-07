#include "nxtOSEKRobotGenerator.h"

#include <QtCore/QTextStream>
#include <cmath>
#include <QtCore/QObject>
#include <QtCore/QDir>

#include "../../../../qrkernel/exception/exception.h"
#include "../../../../qrkernel/settingsManager.h"
#include "../../../../qrutils/inFile.h"
#include "../../../../qrutils/outFile.h"

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
		, mSubprogramsGenerator(this)
		, mMainControlFlowGenerator(this, mDiagram)
		, mCurrentGenerator(&mMainControlFlowGenerator)
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
		, mSubprogramsGenerator(this)
		, mMainControlFlowGenerator(this, mDiagram)
		, mCurrentGenerator(&mMainControlFlowGenerator)
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

QString NxtOSEKRobotGenerator::toString(QList<SmartLine> const &lineList)
{
	QString resultCode;
	foreach (SmartLine const &curLine, lineList) {
		if (curLine.indentLevelChange() == SmartLine::decrease
				|| curLine.indentLevelChange() == SmartLine::decreaseOnlyThisLine)
		{
			--mCurTabNumber;
		}
		resultCode += QString(mCurTabNumber, '\t') + curLine.text() + "\n";
		if (curLine.indentLevelChange() == SmartLine::increase
				|| curLine.indentLevelChange() == SmartLine::decreaseOnlyThisLine)
		{
			++mCurTabNumber;
		}
	}
	return resultCode;
}

void NxtOSEKRobotGenerator::generateMakeFile(QString const &projectName
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
}

void NxtOSEKRobotGenerator::insertCode(
		QString const &resultCode
		, QString const &resultInitCode
		, QString const &resultTerminateCode
		, QString const &resultIsrHooksCode
		, QString const &subprogramsCode
		, QString const &curInitialNodeNumber
		)
{
	if (mBalancerIsActivated) {
		mResultString.replace("@@BALANCER@@", "#include \"balancer.h\"");
	} else {
		mResultString.replace("@@BALANCER@@", "");
	}

	mResultString.replace("@@CODE@@", resultCode +"\n" + "@@CODE@@")
			.replace("@@VARIABLES@@", mVariables.generateVariableString() + "\n@@VARIABLES@@")
			.replace("@@INITHOOKS@@", resultInitCode)
			.replace("@@TERMINATEHOOKS@@", resultTerminateCode)
			.replace("@@USERISRHOOKS@@", resultIsrHooksCode)
			.replace("@@BMPFILES@@", mImageGenerator.generateBmpFilesStringForC()
					+ "@@BMPFILES@@")
			.replace("@@SUBPROGRAMS@@", subprogramsCode);
	mTaskTemplate.replace("@@NUMBER@@", curInitialNodeNumber);
	mResultOil.replace("@@TASK@@", mTaskTemplate + "\n" + "@@TASK@@");
}

void NxtOSEKRobotGenerator::deleteResidualLabels(QString const &projectName)
{
	mResultOil.replace("@@TASK@@", "");
	mResultString.replace("@@VARIABLES@@", "").replace("@@BMPFILES@@", "").replace("@@CODE@@", "").replace("@@PROJECT_NAME@@", projectName);
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

	int curInitialNodeNumber = 0;
	QString const projectName = "example" + QString::number(curInitialNodeNumber);
	QString const projectDir = "nxt-tools/" + projectName;

	initializeGeneration(projectDir);

	QString resultTaskTemplate = utils::InFile::readAll(":/nxtOSEK/templates/taskTemplate.oil");
	initializeFields(resultTaskTemplate);

	if (!mMainControlFlowGenerator.generate() || !mSubprogramsGenerator.generate()) {
		return;
	}

	addResultCodeInCFile(curInitialNodeNumber);
	outputInCAndOilFile(projectName, projectDir);
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

interpreters::robots::enums::sensorType::SensorTypeEnum NxtOSEKRobotGenerator::portValue(int port) const
{
	return static_cast<interpreters::robots::enums::sensorType::SensorTypeEnum>(
			SettingsManager::value("port" + QString::number(port) + "SensorType").toInt()
			);
}

ErrorReporterInterface &NxtOSEKRobotGenerator::errorReporter()
{
	return mErrorReporter;
}

QString NxtOSEKRobotGenerator::intExpression(Id const &id, QString const &propertyName) const
{
	QString const expression = mApi->stringProperty(id, propertyName);
	return mVariables.expressionToInt(expression);
}

void NxtOSEKRobotGenerator::activateBalancer()
{
	mBalancerIsActivated = true;
}

void NxtOSEKRobotGenerator::addResultCodeInCFile(int curInitialNodeNumber)
{
	QString const nodeNumber = QString::number(curInitialNodeNumber);

	// This will add to all next code one tab before each line
	mCurTabNumber = 1;

	QString const resultCode = QString("TASK(OSEK_Task_Number_%1)\n{\n%2}").arg(nodeNumber
			, toString(mMainControlFlowGenerator.generatedCode()));
	QString const resultInitCode = toString(mInitCode);
	QString const resultTerminateCode = toString(mTerminateCode);
	QString const resultIsrHooksCode = toString(mIsrHooksCode);

	// Subprograms have been already generated with first tab consideration
	mCurTabNumber = 0;
	QString const subprogramsCode = toString(mSubprogramsGenerator.generatedCode());

	insertCode(resultCode, resultInitCode, resultTerminateCode, resultIsrHooksCode
			, subprogramsCode, nodeNumber);
}

void NxtOSEKRobotGenerator::outputInCAndOilFile(QString const &projectName, QString const &projectDir)
{
	deleteResidualLabels(projectName);
	//Output in the .c and .oil file
	utils::OutFile outC(projectDir + "/" + projectName + ".c");
	outC() << mResultString;
	utils::OutFile outOil(projectDir + "/" + projectName + ".oil");
	outOil() << mResultOil;
	generateFilesForBalancer(projectDir);
	generateMakeFile(projectName, projectDir);
}

void NxtOSEKRobotGenerator::initializeFields(QString const &resultTaskTemplate)
{
	mTaskTemplate = resultTaskTemplate;
	mVariablePlaceInGenStrSet = 0;
	mVariables.reinit(mApi);
	mBalancerIsActivated = false;
	mImageGenerator.reinit();
}

ImageGenerator &NxtOSEKRobotGenerator::imageGenerator()
{
	return mImageGenerator;
}

SubprogramsGenerator &NxtOSEKRobotGenerator::subprogramsGenerator()
{
	return mSubprogramsGenerator;
}

ControlFlowGenerator *NxtOSEKRobotGenerator::currentGenerator()
{
	return mCurrentGenerator;
}

void NxtOSEKRobotGenerator::beforeSubprogramGeneration(ControlFlowGenerator * const generator)
{
	mCurrentGenerator = generator;
}
