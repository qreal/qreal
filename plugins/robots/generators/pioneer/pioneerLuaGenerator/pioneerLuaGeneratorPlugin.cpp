/* Copyright 2017 QReal Research Group
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

#include "pioneerLuaGeneratorPlugin.h"

#include <QtCore/QProcess>
#include <QtCore/QDir>
#include <QtCore/QTextCodec>

#include <qrkernel/logging.h>
#include <qrkernel/settingsManager.h>
#include <pioneerKit/blocks/pioneerBlocksFactory.h>
#include <pioneerKit/constants.h>

#include "pioneerLuaMasterGenerator.h"
#include "robotModel/pioneerGeneratorRobotModel.h"
#include "widgets/pioneerAdditionalPreferences.h"

using namespace pioneer;
using namespace pioneer::lua;
using namespace qReal;

PioneerLuaGeneratorPlugin::PioneerLuaGeneratorPlugin()
	: mGenerateCodeAction(new QAction(nullptr))
	, mUploadProgramAction(new QAction(nullptr))
	, mRunProgramAction(new QAction(nullptr))
	, mBlocksFactory(new blocks::PioneerBlocksFactory)
	, mGeneratorForRealCopterRobotModel(
			new PioneerGeneratorRobotModel(
					kitId()
					, "Pioneer"
					, modelNames::realCopter
					, tr("Pioneer model (real copter)")
					, 9
				)
		)
	, mGeneratorForSimulatorRobotModel(
		new PioneerGeneratorRobotModel(
				kitId()
				, "Pioneer"
				, modelNames::simulator
				, tr("Pioneer model (simulator)")
				, 10
			)
		)
	, mUploadProcess(new QProcess)
	, mStartProcess(new QProcess)
{
	mAdditionalPreferences = new PioneerAdditionalPreferences;

	mGenerateCodeAction->setText(tr("Generate to Pioneer Lua"));
	mGenerateCodeAction->setIcon(QIcon(":/pioneer/lua/images/generateLuaCode.svg"));
	connect(mGenerateCodeAction, &QAction::triggered, this, &PioneerLuaGeneratorPlugin::generateCode);

	mUploadProgramAction->setText(tr("Upload generated program to Pioneer"));
	mUploadProgramAction->setIcon(QIcon(":/pioneer/lua/images/upload.svg"));
	connect(mUploadProgramAction, &QAction::triggered, this, &PioneerLuaGeneratorPlugin::uploadProgram);

	mRunProgramAction->setText(tr("Run program on a Pioneer"));
	mRunProgramAction->setIcon(QIcon(":/pioneer/lua/images/run.svg"));
	connect(mRunProgramAction, &QAction::triggered, this, &PioneerLuaGeneratorPlugin::runProgram);

	text::Languages::registerLanguage(text::LanguageInfo{ "lua"
			, tr("Lua language")
			, true
			, 4
			, nullptr
			, {}
	});

	connect(mUploadProcess.data()
			, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished)
			, this
			, &PioneerLuaGeneratorPlugin::onUploadCompleted);

	connect(mStartProcess.data()
			, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished)
			, this
			, &PioneerLuaGeneratorPlugin::onStartCompleted);
}

PioneerLuaGeneratorPlugin::~PioneerLuaGeneratorPlugin()
{
	if (mOwnsAdditionalPreferences) {
		delete mAdditionalPreferences;
	}
}

QList<ActionInfo> PioneerLuaGeneratorPlugin::customActions()
{
	const ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	const ActionInfo uploadProgramActionInfo(mUploadProgramAction, "generators", "tools");
	const ActionInfo runProgramActionInfo(mRunProgramAction, "generators", "tools");
	return { generateCodeActionInfo, uploadProgramActionInfo, runProgramActionInfo };
}

QList<HotKeyActionInfo> PioneerLuaGeneratorPlugin::hotKeyActions()
{
	mGenerateCodeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));
	mUploadProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
	mRunProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));

	HotKeyActionInfo generateActionInfo(
			"Generator.GeneratePioneerLua"
			, tr("Generate Lua script for Pioneer Quadcopter")
			, mGenerateCodeAction);

	HotKeyActionInfo uploadProgramInfo(
			"Generator.UploadPioneerLua"
			, tr("Upload Pioneer program")
			, mUploadProgramAction);

	HotKeyActionInfo runProgramInfo("Generator.RunPioneerLua", tr("Run Pioneer Program"), mRunProgramAction);

	return { generateActionInfo, uploadProgramInfo, runProgramInfo };
}

QIcon PioneerLuaGeneratorPlugin::iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon(":/pioneer/lua/images/switchToPioneerGenerator.svg");
}

int PioneerLuaGeneratorPlugin::priority() const
{
	return 9;
}

QString PioneerLuaGeneratorPlugin::kitId() const
{
	return "pioneerKit";
}

QList<kitBase::robotModel::RobotModelInterface *> PioneerLuaGeneratorPlugin::robotModels()
{
	return { mGeneratorForRealCopterRobotModel.data(), mGeneratorForSimulatorRobotModel.data() };
}

kitBase::blocksBase::BlocksFactoryInterface *PioneerLuaGeneratorPlugin::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	Q_UNUSED(model)
	return mBlocksFactory;
}

QList<kitBase::AdditionalPreferences *> PioneerLuaGeneratorPlugin::settingsWidgets()
{
	mOwnsAdditionalPreferences = false;
	return { mAdditionalPreferences };
}

QString PioneerLuaGeneratorPlugin::defaultSettingsFile() const
{
	return ":/pioneer/lua/pioneerLuaDefaultSettings.ini";
}

QString PioneerLuaGeneratorPlugin::defaultFilePath(const QString &projectName) const
{
	return QString("pioneer/%1/%1.lua").arg(projectName);
}

text::LanguageInfo PioneerLuaGeneratorPlugin::language() const
{
	return text::Languages::pickByExtension("lua");
}

QString PioneerLuaGeneratorPlugin::generatorName() const
{
	return "pioneer/lua";
}

generatorBase::MasterGeneratorBase *PioneerLuaGeneratorPlugin::masterGenerator()
{
	return new PioneerLuaMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mParserErrorReporter
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, generatorName());
}

void PioneerLuaGeneratorPlugin::regenerateExtraFiles(const QFileInfo &newFileInfo)
{
	Q_UNUSED(newFileInfo)
}

void PioneerLuaGeneratorPlugin::uploadProgram()
{
	const QString server = SettingsManager::value(settings::pioneerBaseStationIP).toString();
	if (server.isEmpty()) {
		mMainWindowInterface->errorReporter()->addError(
				tr("Pioneer base station IP addres is not set. It can be set in Settings window.")
		);
		return;
	}

	QString pathToPython = SettingsManager::value(settings::pioneerPythonPath).toString();
	if (pathToPython.isEmpty()) {
		pathToPython = "python";
	}

#ifdef Q_OS_WIN
	const QString processName = QApplication::applicationDirPath() + "/pioneerUpload.bat";
#else
	const QString processName = QApplication::applicationDirPath() + "/pioneerUpload.sh";
#endif
	const QFileInfo fileInfo = generateCodeForProcessing();

	QString pathToLuac = mRobotModelManager->model().name() == modelNames::realCopter
			? QApplication::applicationDirPath()
					+ "/"
					+ SettingsManager::value(settings::realCopterLuaPath, "").toString()
			: SettingsManager::value(settings::simulatorLuaPath, "").toString();

	const QString pathToControllerScript = QApplication::applicationDirPath() + "/";

	mUploadProcess->start(processName, {
			fileInfo.absoluteFilePath()
			, server
			, pathToLuac
			, pathToPython
			, pathToControllerScript });

	setUploadAndRunActionsEnabled(false);

	mUploadProcess->waitForStarted();
	if (mUploadProcess->state() != QProcess::Running) {
		mMainWindowInterface->errorReporter()->addError(tr("Unable to execute script"));
		QStringList errors = QString(mUploadProcess->readAllStandardError()).split("\n", QString::SkipEmptyParts);
		for (const auto &error : errors) {
			mMainWindowInterface->errorReporter()->addInformation(error);
		}

		setUploadAndRunActionsEnabled(true);
	} else {
		mMainWindowInterface->errorReporter()->addInformation(tr("Uploading started, please wait..."));
	}
}

void PioneerLuaGeneratorPlugin::runProgram()
{
	mIsStartNeeded = true;
	uploadProgram();
}

void PioneerLuaGeneratorPlugin::doRunProgram()
{
	const QString server = SettingsManager::value(settings::pioneerBaseStationIP).toString();
	if (server.isEmpty()) {
		mMainWindowInterface->errorReporter()->addError(
				tr("Pioneer base station IP addres is not set. It can be set in Settings window.")
		);
		return;
	}

	QString pathToPython = SettingsManager::value(settings::pioneerPythonPath).toString();
	if (pathToPython.isEmpty()) {
		pathToPython = "python";
	}

#ifdef Q_OS_WIN
	const QString processName = QApplication::applicationDirPath() + "/pioneerStart.bat";
#else
	const QString processName = QApplication::applicationDirPath() + "/pioneerStart.sh";
#endif
	const QString pathToControllerScript = QApplication::applicationDirPath() + "/";

	mStartProcess->start(processName, { server, pathToPython, pathToControllerScript });
	mStartProcess->waitForStarted();
	if (mStartProcess->state() != QProcess::Running) {
		mMainWindowInterface->errorReporter()->addError(tr("Unable to execute script"));
		setUploadAndRunActionsEnabled(true);
	} else {
		mMainWindowInterface->errorReporter()->addInformation(tr("Starting program, please wait..."));
	}
}

void PioneerLuaGeneratorPlugin::onUploadCompleted()
{
	QStringList errors = toUnicode(mUploadProcess->readAllStandardError()).split("\n", QString::SkipEmptyParts);
	errors << toUnicode(mUploadProcess->readAllStandardOutput()).split("\n", QString::SkipEmptyParts);
	for (const auto &error : errors) {
		mMainWindowInterface->errorReporter()->addInformation(error);
	}

	mMainWindowInterface->errorReporter()->addInformation(tr("Uploading finished."));

	if (mIsStartNeeded) {
		doRunProgram();
		mIsStartNeeded = false;
	} else {
		setUploadAndRunActionsEnabled(true);
	}
}

void PioneerLuaGeneratorPlugin::onStartCompleted()
{
	QStringList errors = toUnicode(mStartProcess->readAllStandardError()).split("\n", QString::SkipEmptyParts);
	errors << toUnicode(mStartProcess->readAllStandardOutput()).split("\n", QString::SkipEmptyParts);
	for (const auto &error : errors) {
		mMainWindowInterface->errorReporter()->addInformation(error);
	}

	if (!mUploadProgramAction->isEnabled()) {
		mMainWindowInterface->errorReporter()->addInformation(tr("Starting finished."));
		setUploadAndRunActionsEnabled(true);
	}
}

void PioneerLuaGeneratorPlugin::setUploadAndRunActionsEnabled(bool isEnabled)
{
	mUploadProgramAction->setEnabled(isEnabled);
	mRunProgramAction->setEnabled(isEnabled);
}

QString PioneerLuaGeneratorPlugin::toUnicode(const QByteArray &str)
{
#ifdef WIN32
	QTextCodec *codec = QTextCodec::codecForName("cp866");
#else
	QTextCodec *codec = QTextCodec::codecForLocale();
#endif

	return codec ? codec->toUnicode(str) : QString(str);
}
