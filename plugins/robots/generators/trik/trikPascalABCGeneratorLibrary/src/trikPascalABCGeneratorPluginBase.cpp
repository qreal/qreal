/* Copyright 2016 Ivan Limar and CyberTech Labs Ltd.
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

#include "trikPascalABCGeneratorLibrary/trikPascalABCGeneratorPluginBase.h"

#include <QtCore/QProcess>
#include <QtCore/QDir>

#include <qrkernel/platformInfo.h>
#include <qrkernel/settingsManager.h>
#include <trikGeneratorBase/trikGeneratorPluginBase.h>
#include <trikGeneratorBase/robotModel/generatorModelExtensionInterface.h>
#include <utils/robotCommunication/tcpRobotCommunicator.h>
#include <utils/robotCommunication/networkCommunicationErrorReporter.h>

#include "trikPascalABCMasterGenerator.h"
#include "trikPascalABCAdditionalPreferences.h"

using namespace trik::pascalABC;
using namespace kitBase::robotModel;
using namespace qReal;
using namespace utils::robotCommunication;

#ifdef Q_OS_WIN

const QString moveCommand = "synchronize remote . /home/root/trik";
const QStringList commands = { moveCommand };

#else

const QString copyCommand = "scp -r -v -oConnectTimeout=%SSH_TIMEOUT%s -oStrictHostKeyChecking=no "
		"-oUserKnownHostsFile=/dev/null %PATH%/* root@%IP%:/home/root/trik";

const QStringList commands = { copyCommand };

#endif

TrikPascalABCGeneratorPluginBase::TrikPascalABCGeneratorPluginBase(
		kitBase::robotModel::RobotModelInterface * const robotModel
		, kitBase::blocksBase::BlocksFactoryInterface * const blocksFactory
		, const QStringList &pathsToTemplates)
	: TrikGeneratorPluginBase(robotModel, blocksFactory)
	, mGenerateCodeAction(new QAction(nullptr))
	, mUploadProgramAction(new QAction(nullptr))
	, mRunProgramAction(new QAction(nullptr))
	, mStopRobotAction(new QAction(nullptr))
	, mAdditionalPreferences(new TrikPascalABCAdditionalPreferences(robotModel->name()))
	, mPathsToTemplates(pathsToTemplates)
	, mRuntimeUploaderTool(
			tr("Upload Pascal Runtime")
			, ":/trik/pascalABC/images/flashRobot.svg"
			/// @todo: hmmm
			, "trikV62Kit"
			, ::commands
			, QObject::tr("Attention! Started to download Pascal runtime."
					" Please do not turn off the robot.")
			, [](){ return qReal::SettingsManager::value("TrikTcpServer").toString(); }
			)
{
}

TrikPascalABCGeneratorPluginBase::~TrikPascalABCGeneratorPluginBase()
{
	if (mOwnsAdditionalPreferences) {
		delete mAdditionalPreferences;
	}
}

QList<ActionInfo> TrikPascalABCGeneratorPluginBase::customActions()
{
	mGenerateCodeAction->setObjectName("generatePascalABCCode");
	mGenerateCodeAction->setText(tr("Generate pascalABC code"));
	mGenerateCodeAction->setIcon(QIcon(":/trik/pascalABC/images/generateCode.svg"));
	ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	connect(mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()), Qt::UniqueConnection);

	mUploadProgramAction->setObjectName("uploadProgram");
	mUploadProgramAction->setText(tr("Upload program"));
	mUploadProgramAction->setIcon(QIcon(":/trik/pascalABC/images/uploadProgram.svg"));
	ActionInfo uploadProgramActionInfo(mUploadProgramAction, "generators", "tools");
	connect(mUploadProgramAction, SIGNAL(triggered()), this, SLOT(uploadProgram()), Qt::UniqueConnection);

	mRunProgramAction->setObjectName("runProgram");
	mRunProgramAction->setText(tr("Run program"));
	mRunProgramAction->setIcon(QIcon(":/trik/pascalABC/images/run.png"));
	ActionInfo runProgramActionInfo(mRunProgramAction, "interpreters", "tools");
	connect(mRunProgramAction, SIGNAL(triggered()), this, SLOT(runProgram()), Qt::UniqueConnection);

	mStopRobotAction->setObjectName("stopRobot");
	mStopRobotAction->setText(tr("Stop robot"));
	mStopRobotAction->setIcon(QIcon(":/trik/pascalABC/images/stop.png"));
	ActionInfo stopRobotActionInfo(mStopRobotAction, "interpreters", "tools");
	connect(mStopRobotAction, SIGNAL(triggered()), this, SLOT(stopRobot()), Qt::UniqueConnection);

	return {generateCodeActionInfo
			, uploadProgramActionInfo
			, runProgramActionInfo
			, stopRobotActionInfo
			, mRuntimeUploaderTool.action()
	};
}

QList<qReal::HotKeyActionInfo> TrikPascalABCGeneratorPluginBase::hotKeyActions()
{
	mGenerateCodeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
	mUploadProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
	mRunProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F6));
	mStopRobotAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F6));

	HotKeyActionInfo generateCodeInfo("Generator.GeneratePascal", tr("Generate Pascal Code"), mGenerateCodeAction);
	HotKeyActionInfo uploadProgramInfo("Generator.UploadPascal", tr("Upload Pascal Program"), mUploadProgramAction);
	HotKeyActionInfo runProgramInfo("Generator.RunPascal", tr("Run Pascal Program"), mRunProgramAction);
	HotKeyActionInfo stopRobotInfo("Generator.StopPascal", tr("Stop Pascal Program"), mStopRobotAction);

	return {generateCodeInfo, uploadProgramInfo, runProgramInfo, stopRobotInfo};
}

QIcon TrikPascalABCGeneratorPluginBase::iconForFastSelector(const RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon(":/trik/pascalABC/images/switch-to-trik-pascal.svg");
}

QList<kitBase::AdditionalPreferences *> TrikPascalABCGeneratorPluginBase::settingsWidgets()
{
	mOwnsAdditionalPreferences = false;
	return {mAdditionalPreferences};
}

void TrikPascalABCGeneratorPluginBase::init(const kitBase::KitPluginConfigurator &configurator)
{
	TrikGeneratorPluginBase::init(configurator);

	ErrorReporterInterface &errorReporter =
			*configurator.qRealConfigurator().mainWindowInterpretersInterface().errorReporter();

	mRuntimeUploaderTool.init(configurator.qRealConfigurator().mainWindowInterpretersInterface()
		, qReal::PlatformInfo::invariantSettingsPath("pathToPascalRuntime"));

	mCommunicator.reset(new TcpRobotCommunicator("TrikTcpServer"));
	NetworkCommunicationErrorReporter::connectErrorReporter(*mCommunicator, errorReporter);

	mStopRobotProtocol.reset(new StopRobotProtocol(*mCommunicator));

	connect(mStopRobotProtocol.data(), &StopRobotProtocol::timeout, [this, &errorReporter]() {
		errorReporter.addError(tr("Stop robot operation timed out"));
	});
}

generatorBase::MasterGeneratorBase *TrikPascalABCGeneratorPluginBase::masterGenerator()
{
	return new TrikPascalABCMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mParserErrorReporter
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, mPathsToTemplates);
}

QString TrikPascalABCGeneratorPluginBase::defaultFilePath(const QString &projectName) const
{
	return QString("trik/%1/%1.pas").arg(projectName);
}

text::LanguageInfo TrikPascalABCGeneratorPluginBase::language() const
{
	return qReal::text::Languages::pascalABC({"robot"});
}

QString TrikPascalABCGeneratorPluginBase::generatorName() const
{
	return "trikPascalABC";
}

QString TrikPascalABCGeneratorPluginBase::uploadProgram()
{
	QProcess compileProcess;
	const QFileInfo fileInfo = generateCodeForProcessing();

	const QString pascalCompiler = qReal::SettingsManager::value("PascalABCPath").toString();

	if (pascalCompiler.isEmpty()) {
		mMainWindowInterface->errorReporter()->addError(
			tr("Please provide path to the PascalABC.NET Compiler in Settings dialog.")
		);

		return "";
	}

	mMainWindowInterface->errorReporter()->addInformation(
		tr("Compiling...")
	);

	compileProcess.setWorkingDirectory(fileInfo.absoluteDir().path());

#ifdef Q_OS_WIN
	compileProcess.start("cmd", {"/C", "start", "PascalABC Compiler", pascalCompiler, fileInfo.absoluteFilePath()});
#else
	compileProcess.start("mono", {pascalCompiler, fileInfo.absoluteFilePath()});
#endif

	compileProcess.waitForStarted();
	if (compileProcess.state() != QProcess::Running) {
		mMainWindowInterface->errorReporter()->addError(tr("Unable to launch PascalABC.NET compiler"));
		return "";
	}

	compileProcess.waitForFinished();

	/// @todo: will not work since PascalABC uses console device instead of stdout or stderr for error output, so
	///        it will always return exit code 0 (even when using console command that actually captures exit code,
	///        start cmd /c "pabcnetc.exe <file name>.pas || call echo %errorLevel% > exitcode.txt"
	///        Need to patch PascalABC.NET compiler to fix that. Or maybe it already can do it, but more investigation
	///        is needed.
	if (compileProcess.exitCode() != 0) {
		mMainWindowInterface->errorReporter()->addError(tr("PascalABC compiler finished with error."));
		QStringList errors = QString(compileProcess.readAllStandardError()).split("\n", QString::SkipEmptyParts);
		errors << QString(compileProcess.readAllStandardOutput()).split("\n", QString::SkipEmptyParts);
		for (const auto &error : errors) {
			mMainWindowInterface->errorReporter()->addInformation(error);
		}

		return "";
	}

	/// @todo: dirty hack. "start" launches process detached so we don't even know when compiler finishes.
	QEventLoop eventLoop;
	QTimer::singleShot(2000, &eventLoop, &QEventLoop::quit);
	eventLoop.exec();
	mMainWindowInterface->errorReporter()->addInformation(
		tr("Compile completed"));

#ifdef Q_OS_WIN
	if (qReal::SettingsManager::value("WinScpPath").toString().isEmpty()) {
		mMainWindowInterface->errorReporter()->addError(
			tr("Please provide path to the WinSCP in Settings dialog.")
		);

		return "";
	}
#endif

	const QFileInfo binaryFile(fileInfo.canonicalPath() + "/" + fileInfo.completeBaseName() + ".exe");
	QFile shScript(fileInfo.canonicalPath() + "/" + fileInfo.completeBaseName() + ".sh");
	shScript.open(QIODevice::WriteOnly);
	const QString script = "mono /home/root/trik/" + binaryFile.fileName();
	shScript.write(script.toStdString().data());
	shScript.close();
	const QFileInfo scriptFile = QFileInfo(shScript);
	const QString scriptPath = scriptFile.canonicalFilePath();

#ifdef Q_OS_WIN
	const QString moveBinary = QString(
			"\"%1\" /command  \"open scp://root@%2\" \"put %3 /home/root/trik/\"")
			.arg(qReal::SettingsManager::value("WinScpPath").toString())
			.arg(qReal::SettingsManager::value("TrikTcpServer").toString())
			.arg(binaryFile.canonicalFilePath().replace("/", "\\"));
#else
	//todo chmod and moveScript for windows
	const QString chmod = QString("chmod +x %1").arg(scriptPath);
	const QString moveScript = QString("scp %2 root@%1:/home/root/trik/scripts")
			.arg(qReal::SettingsManager::value("TrikTcpServer").toString())
			.arg(scriptPath);
	QProcess chmodProcess;
	chmodProcess.start(chmod);
	chmodProcess.waitForFinished();
	QProcess moveScriptProcess;
	moveScriptProcess.start(moveScript);
	moveScriptProcess.waitForFinished(3000);
	const QString moveBinary = QString(
				"scp %2 root@%1:/home/root/trik")
				.arg(qReal::SettingsManager::value("TrikTcpServer").toString())
				.arg(binaryFile.canonicalFilePath());
#endif

	mMainWindowInterface->errorReporter()->addInformation(
		tr("Uploading... Please wait for about 20 seconds.")
	);

	QProcess deployProcess;

	//check on windows
	deployProcess.start(moveBinary);

	if (deployProcess.state() == QProcess::NotRunning) {
		mMainWindowInterface->errorReporter()->addError(
					tr("Cant't start download process"));
		return "";
	}

	mMainWindowInterface->errorReporter()->addInformation(
		tr("After downloading the program, enter 'exit' or close the window")
	);

	if (deployProcess.exitCode() != 0 or !deployProcess.waitForFinished(10000))
	{
		mMainWindowInterface->errorReporter()->addError(
					tr("Failed to download program. Check connection"));
		return "";
	}

	mMainWindowInterface->errorReporter()->addInformation(
				tr("Download completed"));

	return binaryFile.fileName();
}

void TrikPascalABCGeneratorPluginBase::runProgram()
{
	const QString binary = uploadProgram();
	if (binary.isEmpty()) {
		return;
	}

	mMainWindowInterface->errorReporter()->addWarning(
		tr("Running... Attention, program execution will start after about ten seconds")
	);

	mCommunicator->runDirectCommand("script.system(\"mono /home/root/trik/" + binary + "\"); ");
}

void TrikPascalABCGeneratorPluginBase::stopRobot()
{
	mStopRobotProtocol->run(
			"script.system(\"killall mono\"); "
			"script.system(\"killall aplay\"); \n"
			"script.system(\"killall vlc\");"
		);
}
