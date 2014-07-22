#include "fSharpGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <qrkernel/settingsManager.h>
#include <qrutils/invocationUtils/functorOperation.h>

#include <QtCore/QDebug>

#include <utils/tcpRobotCommunicator.h>
#include "fSharpMasterGenerator.h"


using namespace fSharp;
using namespace qReal;

FSharpGeneratorPlugin::FSharpGeneratorPlugin()
		: mGenerateCodeAction(nullptr)
		, mUploadProgramAction(nullptr)
		, mRunProgramAction(nullptr)
		, mStopRobotAction(nullptr)
{
	mAppTranslator.load(":/FSharpGenerator_" + QLocale().name());
	QApplication::installTranslator(&mAppTranslator);
}

FSharpGeneratorPlugin::~FSharpGeneratorPlugin()
{
}

QString FSharpGeneratorPlugin::kitId() const
{
	return "trikKit";
}

QList<ActionInfo> FSharpGeneratorPlugin::actions()
{
	mGenerateCodeAction.setText(tr("Generate FSharp code"));
	mGenerateCodeAction.setIcon(QIcon(":/images/generateQtsCode.svg"));
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));

	mUploadProgramAction.setText(tr("Upload program FSharp"));
	mUploadProgramAction.setIcon(QIcon(":/images/uploadProgram.svg"));
	ActionInfo uploadProgramActionInfo(&mUploadProgramAction, "generators", "tools");
	connect(&mUploadProgramAction, SIGNAL(triggered()), this, SLOT(uploadProgram()));

	mRunProgramAction.setText(tr("Run program FSharp"));
	mRunProgramAction.setIcon(QIcon(":/images/uploadAndExecuteProgram.svg"));
	ActionInfo runProgramActionInfo(&mRunProgramAction, "generators", "tools");
	connect(&mRunProgramAction, SIGNAL(triggered()), this, SLOT(runProgram()));

	mStopRobotAction.setText(tr("Stop robot"));
	mStopRobotAction.setIcon(QIcon(":/images/stopRobot.svg"));
	ActionInfo stopRobotActionInfo(&mStopRobotAction, "generators", "tools");
	connect(&mStopRobotAction, SIGNAL(triggered()), this, SLOT(stopRobot()));

	return {generateCodeActionInfo, uploadProgramActionInfo, runProgramActionInfo, stopRobotActionInfo};
}

QList<HotKeyActionInfo> FSharpGeneratorPlugin::hotKeyActions()
{
	mGenerateCodeAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
	mUploadProgramAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
	mRunProgramAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F6));
	mStopRobotAction.setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F6));

	HotKeyActionInfo generateCodeInfo("Generator.GenerateFSharp", tr("Generate FSharp Code"), &mGenerateCodeAction);
	HotKeyActionInfo uploadProgramInfo("Generator.UploadFSharp", tr("Upload FSharp Program"), &mUploadProgramAction);
	HotKeyActionInfo runProgramInfo("Generator.RunFSharp", tr("Run FSharp Program"), &mRunProgramAction);
	HotKeyActionInfo stopRobotInfo("Generator.StopFSharp", tr("Stop FSharp Robot"), &mStopRobotAction);

	return {generateCodeInfo, uploadProgramInfo, runProgramInfo, stopRobotInfo};
}

generatorBase::MasterGeneratorBase *FSharpGeneratorPlugin::masterGenerator()
{
	return new FSharpMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mRobotModelManager
			, mMainWindowInterface->activeDiagram());
}

void FSharpGeneratorPlugin::regenerateExtraFiles(QFileInfo const &newFileInfo)
{
	Q_UNUSED(newFileInfo);
}

QString FSharpGeneratorPlugin::defaultFilePath(QString const &projectName) const
{
	return QString("trik/%1/%1.fs").arg(projectName);
}

QString FSharpGeneratorPlugin::extension() const
{
	return "fs";
}

QString FSharpGeneratorPlugin::extDescrition() const
{
	return tr("FSharp Source File");
}

QString FSharpGeneratorPlugin::generatorName() const
{
	return "FSharp";
}

bool FSharpGeneratorPlugin::uploadProgram()
{
	QProcess myProcess;
	QFileInfo const fileInfo = generateCodeForProcessing();

	QString pathToTheTrikCore = " -r \"..\\..\\Trik.Core.dll\"";

	if (qReal::SettingsManager::value("FSharpPath").toString() == "") {
		mMainWindowInterface->errorReporter()->addError(
			tr("Input path to the FSharp Compiler!!!")
		);
		return 0;
	}

	QString command = "\"" + qReal::SettingsManager::value("FSharpPath").toString() + "\" "
			+ "\"" + fileInfo.absoluteFilePath() + "\""
			+ pathToTheTrikCore;

	myProcess.setWorkingDirectory(fileInfo.absoluteDir().path());
	myProcess.start(command);
	myProcess.waitForFinished();

	if (qReal::SettingsManager::value("WinScpPath").toString() == "") {
		mMainWindowInterface->errorReporter()->addError(
			tr("Input path to the WinSCP!!!")
		);
		return 0;
	}

	QString moveCommand = "\"" + qReal::SettingsManager::value("WinScpPath").toString() + "\""
			+ " /command  \"open scp://root@" + qReal::SettingsManager::value("TrikTcpServer").toString() + "\""
			+ " \"put "+ fileInfo.absoluteFilePath().replace("fs","exe").replace("/","\\")
			+ " /home/root/trik/FSharp/Environment/\"";

	myProcess.startDetached(moveCommand);
	//myProcess.waitForFinished();
	mMainWindowInterface->errorReporter()->addInformation(
		tr("After downloading the program, enter 'exit' or close the window")
	);

	return 1;
}

void FSharpGeneratorPlugin::runProgram()
{
	mMainWindowInterface->errorReporter()->addWarning(
		tr("Attention, the robot starts about a half-minute")
	);

	utils::TcpRobotCommunicator communicator("TrikTcpServer");

	communicator.runDirectCommand(
			"brick.system(\"mono FSharp/Environment/example0.exe\"); "
	);

}

void FSharpGeneratorPlugin::stopRobot()
{
	utils::TcpRobotCommunicator communicator("TrikTcpServer");

	if (!communicator.stopRobot()) {
		mMainWindowInterface->errorReporter()->addError(tr("No connection to robot"));
	}

	communicator.runDirectCommand(
			"brick.system(\"killall mono FSharp/Environment/example0.exe\"); "
			"brick.system(\"killall aplay\"); \n"
			"brick.system(\"killall vlc\"); \n"
			"brick.system(\"killall rover-cv\");"
	);

}
