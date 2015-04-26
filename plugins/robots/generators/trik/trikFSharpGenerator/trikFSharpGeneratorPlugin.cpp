/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "trikFSharpGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtCore/QProcess>

#include <qrkernel/settingsManager.h>
#include <utils/tcpRobotCommunicator.h>

#include "trikFSharpMasterGenerator.h"

using namespace trik::fSharp;
using namespace qReal;

const QString robotModelName = "TrikFSharpGeneratorRobotModel";

TrikFSharpGeneratorPlugin::TrikFSharpGeneratorPlugin()
	: TrikGeneratorPluginBase(robotModelName, tr("Generation (F#)"), 7 /* Last order */)
	, mGenerateCodeAction(new QAction(nullptr))
	, mUploadProgramAction(new QAction(nullptr))
	, mRunProgramAction(new QAction(nullptr))
	, mStopRobotAction(new QAction(nullptr))
	, mAdditionalPreferences(new TrikFSharpAdditionalPreferences(robotModelName))
{
	mGenerateCodeAction->setText(tr("Generate FSharp code"));
	mGenerateCodeAction->setIcon(QIcon(":/fSharp/images/generateFsCode.svg"));
	connect(mGenerateCodeAction, &QAction::triggered, this, &TrikFSharpGeneratorPlugin::generateCode);

	mUploadProgramAction->setText(tr("Upload program FSharp"));
	mUploadProgramAction->setIcon(QIcon(":/fSharp/images/uploadProgram.svg"));
	connect(mUploadProgramAction, &QAction::triggered, this, &TrikFSharpGeneratorPlugin::uploadProgram);

	mRunProgramAction->setText(tr("Run program FSharp"));
	mRunProgramAction->setIcon(QIcon(":/fSharp/images/run.png"));
	connect(mRunProgramAction, &QAction::triggered, this, &TrikFSharpGeneratorPlugin::runProgram);

	mStopRobotAction->setText(tr("Stop robot"));
	mStopRobotAction->setIcon(QIcon(":/fSharp/images/stop.png"));
	connect(mStopRobotAction, &QAction::triggered, this, &TrikFSharpGeneratorPlugin::stopRobot);
}

TrikFSharpGeneratorPlugin::~TrikFSharpGeneratorPlugin()
{
	if (mOwnsAdditionalPreferences) {
		delete mAdditionalPreferences;
	}
}

QList<ActionInfo> TrikFSharpGeneratorPlugin::customActions()
{
	const ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	const ActionInfo uploadProgramActionInfo(mUploadProgramAction, "generators", "tools");
	const ActionInfo runProgramActionInfo(mRunProgramAction, "interpreters", "tools");
	const ActionInfo stopRobotActionInfo(mStopRobotAction, "interpreters", "tools");
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

QIcon TrikFSharpGeneratorPlugin::iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon(":/fSharp/images/switch-to-trik-f-sharp.svg");
}

QList<kitBase::AdditionalPreferences *> TrikFSharpGeneratorPlugin::settingsWidgets()
{
	mOwnsAdditionalPreferences = false;
	return {mAdditionalPreferences};
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
