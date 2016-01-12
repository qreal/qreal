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

#include "trikFSharpGeneratorLibrary/trikFSharpGeneratorPluginBase.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtCore/QProcess>

#include <qrkernel/settingsManager.h>
#include <utils/robotCommunication/tcpRobotCommunicator.h>
#include <utils/robotCommunication/stopRobotProtocol.h>
#include <utils/robotCommunication/networkCommunicationErrorReporter.h>

#include "trikFSharpMasterGenerator.h"
#include "trikFSharpAdditionalPreferences.h"

using namespace trik::fSharp;
using namespace qReal;
using namespace utils::robotCommunication;

const QString robotModelName = "TrikFSharpGeneratorRobotModel";

TrikFSharpGeneratorPluginBase::TrikFSharpGeneratorPluginBase(
		kitBase::robotModel::RobotModelInterface * const robotModel
		, kitBase::blocksBase::BlocksFactoryInterface * const blocksFactory
		, const QStringList &pathsToTemplates)
	: TrikGeneratorPluginBase(robotModel, blocksFactory)
	, mGenerateCodeAction(new QAction(nullptr))
	, mUploadProgramAction(new QAction(nullptr))
	, mRunProgramAction(new QAction(nullptr))
	, mStopRobotAction(new QAction(nullptr))
	, mAdditionalPreferences(new TrikFSharpAdditionalPreferences(robotModelName))
	, mPathsToTemplates(pathsToTemplates)
{
	mGenerateCodeAction->setText(tr("Generate FSharp code"));
	mGenerateCodeAction->setIcon(QIcon(":/fSharp/images/generateFsCode.svg"));
	connect(mGenerateCodeAction, &QAction::triggered, this, &TrikFSharpGeneratorPluginBase::generateCode);

	mUploadProgramAction->setText(tr("Upload program FSharp"));
	mUploadProgramAction->setIcon(QIcon(":/fSharp/images/uploadProgram.svg"));
	connect(mUploadProgramAction, &QAction::triggered, this, &TrikFSharpGeneratorPluginBase::uploadProgram);

	mRunProgramAction->setText(tr("Run program FSharp"));
	mRunProgramAction->setIcon(QIcon(":/fSharp/images/run.png"));
	connect(mRunProgramAction, &QAction::triggered, this, &TrikFSharpGeneratorPluginBase::runProgram);

	mStopRobotAction->setText(tr("Stop robot"));
	mStopRobotAction->setIcon(QIcon(":/fSharp/images/stop.png"));
	connect(mStopRobotAction, &QAction::triggered, this, &TrikFSharpGeneratorPluginBase::stopRobot);
}

TrikFSharpGeneratorPluginBase::~TrikFSharpGeneratorPluginBase()
{
	if (mOwnsAdditionalPreferences) {
		delete mAdditionalPreferences;
	}
}

void TrikFSharpGeneratorPluginBase::init(const kitBase::KitPluginConfigurator &configurer)
{
	const auto errorReporter = configurer.qRealConfigurator().mainWindowInterpretersInterface().errorReporter();
	RobotsGeneratorPluginBase::init(configurer);
	mCommunicator.reset(new TcpRobotCommunicator("TrikTcpServer"));
	NetworkCommunicationErrorReporter::connectErrorReporter(*mCommunicator, *errorReporter);
	mStopRobotProtocol.reset(new StopRobotProtocol(*mCommunicator));

	connect(mStopRobotProtocol.data(), &StopRobotProtocol::timeout, [errorReporter]() {
		errorReporter->addError(tr("Stop robot operation timed out"));
	});
}

QList<ActionInfo> TrikFSharpGeneratorPluginBase::customActions()
{
	const ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	const ActionInfo uploadProgramActionInfo(mUploadProgramAction, "generators", "tools");
	const ActionInfo runProgramActionInfo(mRunProgramAction, "interpreters", "tools");
	const ActionInfo stopRobotActionInfo(mStopRobotAction, "interpreters", "tools");
	return {generateCodeActionInfo, uploadProgramActionInfo, runProgramActionInfo, stopRobotActionInfo};
}

QList<HotKeyActionInfo> TrikFSharpGeneratorPluginBase::hotKeyActions()
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

QIcon TrikFSharpGeneratorPluginBase::iconForFastSelector(
		const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon(":/fSharp/images/switch-to-trik-f-sharp.svg");
}

QList<kitBase::AdditionalPreferences *> TrikFSharpGeneratorPluginBase::settingsWidgets()
{
	mOwnsAdditionalPreferences = false;
	return {mAdditionalPreferences};
}

generatorBase::MasterGeneratorBase *TrikFSharpGeneratorPluginBase::masterGenerator()
{
	return new TrikFSharpMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mParserErrorReporter
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, mPathsToTemplates);
}

QString TrikFSharpGeneratorPluginBase::defaultFilePath(const QString &projectName) const
{
	return QString("trik/%1/%1.fs").arg(projectName);
}

text::LanguageInfo TrikFSharpGeneratorPluginBase::language() const
{
	return qReal::text::Languages::fSharp({ "robot" });
}

QString TrikFSharpGeneratorPluginBase::generatorName() const
{
	return "trikFSharp";
}

bool TrikFSharpGeneratorPluginBase::uploadProgram()
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

void TrikFSharpGeneratorPluginBase::runProgram()
{
	mMainWindowInterface->errorReporter()->addWarning(
		tr("Attention, the robot starts about a half-minute")
	);

	mCommunicator->runDirectCommand(
			"script.system(\"mono FSharp/Environment/example0.exe\"); "
	);
}

void TrikFSharpGeneratorPluginBase::stopRobot()
{
	mStopRobotProtocol->run(
			"script.system(\"killall mono\"); "
			"script.system(\"killall aplay\"); \n"
			"script.system(\"killall vlc\");"
		);
}
