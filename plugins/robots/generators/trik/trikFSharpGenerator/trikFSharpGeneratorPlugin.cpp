#include "trikFSharpGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>

#include <qrkernel/settingsManager.h>
#include <utils/tcpRobotCommunicator.h>

#include "trikFSharpMasterGenerator.h"

using namespace trik::fSharp;
using namespace qReal;

TrikFSharpGeneratorPlugin::TrikFSharpGeneratorPlugin()
	: mGenerateCodeAction(new QAction(nullptr))
	, mUploadProgramAction(new QAction(nullptr))
	, mRunProgramAction(new QAction(nullptr))
	, mStopRobotAction(new QAction(nullptr))
{
}

QList<ActionInfo> TrikFSharpGeneratorPlugin::actions()
{
	mGenerateCodeAction->setText(tr("Generate FSharp code"));
	mGenerateCodeAction->setIcon(QIcon(":/fSharp/images/generateFsCode.svg"));
	ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	connect(mGenerateCodeAction, &QAction::triggered, this, &TrikFSharpGeneratorPlugin::generateCode);

	mUploadProgramAction->setText(tr("Upload program FSharp"));
	mUploadProgramAction->setIcon(QIcon(":/fSharp/images/uploadProgram.svg"));
	ActionInfo uploadProgramActionInfo(mUploadProgramAction, "generators", "tools");
	connect(mUploadProgramAction, &QAction::triggered, this, &TrikFSharpGeneratorPlugin::uploadProgram);

	mRunProgramAction->setText(tr("Run program FSharp"));
	mRunProgramAction->setIcon(QIcon(":/fSharp/images/uploadAndExecuteProgram.svg"));
	ActionInfo runProgramActionInfo(mRunProgramAction, "generators", "tools");
	connect(mRunProgramAction, &QAction::triggered, this, &TrikFSharpGeneratorPlugin::runProgram);

	mStopRobotAction->setText(tr("Stop robot"));
	mStopRobotAction->setIcon(QIcon(":/fSharp/images/stopRobot.svg"));
	ActionInfo stopRobotActionInfo(mStopRobotAction, "generators", "tools");
	connect(mStopRobotAction, &QAction::triggered, this, &TrikFSharpGeneratorPlugin::stopRobot);

	return {generateCodeActionInfo, uploadProgramActionInfo, runProgramActionInfo, stopRobotActionInfo};
}

QList<HotKeyActionInfo> TrikFSharpGeneratorPlugin::hotKeyActions()
{
	mGenerateCodeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
	mUploadProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
	mRunProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F6));
	mStopRobotAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F6));

	HotKeyActionInfo generateCodeInfo("Generator.GenerateFSharp", tr("Generate FSharp Code"), mGenerateCodeAction);
	HotKeyActionInfo uploadProgramInfo("Generator.UploadFSharp", tr("Upload FSharp Program"), mUploadProgramAction);
	HotKeyActionInfo runProgramInfo("Generator.RunFSharp", tr("Run FSharp Program"), mRunProgramAction);
	HotKeyActionInfo stopRobotInfo("Generator.StopFSharp", tr("Stop FSharp Robot"), mStopRobotAction);

	return {generateCodeInfo, uploadProgramInfo, runProgramInfo, stopRobotInfo};
}

generatorBase::MasterGeneratorBase *TrikFSharpGeneratorPlugin::masterGenerator()
{
	return new TrikFSharpMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mParserErrorReporter
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, generatorName());
}

QString TrikFSharpGeneratorPlugin::defaultFilePath(const QString &projectName) const
{
	return QString("trik/%1/%1.fs").arg(projectName);
}

text::LanguageInfo TrikFSharpGeneratorPlugin::language() const
{
	return qReal::text::Languages::fSharp({ "robot" });
}

QString TrikFSharpGeneratorPlugin::generatorName() const
{
	return "trikFSharp";
}

bool TrikFSharpGeneratorPlugin::uploadProgram()
{
	QProcess compileProcess;
	const QFileInfo fileInfo = generateCodeForProcessing();

	const QString pathToTheTrikCore = " -r \"..\\..\\Trik.Core.dll\"";

	if (qReal::SettingsManager::value("FSharpPath").toString().isEmpty()) {
		mMainWindowInterface->errorReporter()->addError(
			tr("Please provide path to the FSharp Compiler in Settings dialog.")
		);

		return false;
	}

	const QString compileCommand = QString("\"%1\" \"%2\" %3")
			.arg(qReal::SettingsManager::value("FSharpPath").toString())
			.arg(fileInfo.absoluteFilePath())
			.arg(pathToTheTrikCore);

	compileProcess.setWorkingDirectory(fileInfo.absoluteDir().path());
	compileProcess.start(compileCommand);
	compileProcess.waitForStarted();
	if (compileProcess.state() != QProcess::Running) {
		mMainWindowInterface->errorReporter()->addError(tr("Unable to launch F# compiler"));
		return false;
	}

	compileProcess.waitForFinished();

	if (qReal::SettingsManager::value("WinScpPath").toString().isEmpty()) {
		mMainWindowInterface->errorReporter()->addError(
			tr("Please provide path to the WinSCP in Settings dialog.")
		);

		return false;
	}

	const QString moveCommand = QString(
			"\"%1\" /command  \"open scp://root@%2\" \"put %3 /home/root/trik/FSharp/Environment/\"")
			.arg(qReal::SettingsManager::value("WinScpPath").toString())
			.arg(qReal::SettingsManager::value("TrikTcpServer").toString())
			.arg(fileInfo.absoluteFilePath().replace("fs","exe").replace("/","\\"));

	QProcess deployProcess;
	if (!deployProcess.startDetached(moveCommand)) {
		mMainWindowInterface->errorReporter()->addError(tr("Unable to launch WinSCP"));
		return false;
	}

	mMainWindowInterface->errorReporter()->addInformation(
		tr("After downloading the program, enter 'exit' or close the window")
	);

	return true;
}

void TrikFSharpGeneratorPlugin::runProgram()
{
	mMainWindowInterface->errorReporter()->addWarning(
		tr("Attention, the robot starts about a half-minute")
	);

	utils::TcpRobotCommunicator communicator("TrikTcpServer");

	communicator.runDirectCommand(
			"script.system(\"mono FSharp/Environment/example0.exe\"); "
	);
}

void TrikFSharpGeneratorPlugin::stopRobot()
{
	utils::TcpRobotCommunicator communicator("TrikTcpServer");

	if (!communicator.stopRobot()) {
		mMainWindowInterface->errorReporter()->addError(tr("No connection to robot"));
	}

	communicator.runDirectCommand(
			"script.system(\"killall mono\"); "
			"script.system(\"killall aplay\"); \n"
			"script.system(\"killall vlc\");"
	);
}
