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

#include <qrkernel/settingsManager.h>
#include <trikGeneratorBase/trikGeneratorPluginBase.h>
#include <trikGeneratorBase/robotModel/generatorModelExtensionInterface.h>
#include <utils/robotCommunication/tcpRobotCommunicator.h>

#include "trikPascalABCMasterGenerator.h"
#include "trikPascalABCAdditionalPreferences.h"

using namespace trik::pascalABC;
using namespace kitBase::robotModel;
using namespace qReal;

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

	return {generateCodeActionInfo, uploadProgramActionInfo, runProgramActionInfo, stopRobotActionInfo};
}

///@todo: hotKeyActions
QList<qReal::HotKeyActionInfo> TrikPascalABCGeneratorPluginBase::hotKeyActions()
{
	return {};
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

bool TrikPascalABCGeneratorPluginBase::uploadProgram()
{
	QProcess compileProcess;
	const QFileInfo fileInfo = generateCodeForProcessing();

	if (qReal::SettingsManager::value("PascalABCPath").toString().isEmpty()) {
		mMainWindowInterface->errorReporter()->addError(
			tr("Please provide path to the PascalABC.NET Compiler in Settings dialog.")
		);

		return false;
	}

	const QString compileCommand = QString("\"%1\"")
			.arg(qReal::SettingsManager::value("PascalABCPath").toString());

	compileProcess.setWorkingDirectory(fileInfo.absoluteDir().path());
	compileProcess.setArguments({fileInfo.absoluteFilePath()});
	compileProcess.start(compileCommand);
	compileProcess.waitForStarted();
	if (compileProcess.state() != QProcess::Running) {
		mMainWindowInterface->errorReporter()->addError(tr("Unable to launch PascalABC.NET compiler"));
		return false;
	}

	compileProcess.waitForFinished();

	qDebug() << compileProcess.exitCode();
	qDebug() << compileProcess.readAllStandardError();
	qDebug() << compileProcess.readAllStandardOutput();

	if (qReal::SettingsManager::value("WinScpPath").toString().isEmpty()) {
		mMainWindowInterface->errorReporter()->addError(
			tr("Please provide path to the WinSCP in Settings dialog.")
		);

		return false;
	}

	const QString moveCommand = QString(
			"\"%1\" /command  \"open scp://root@%2\" \"put %3 /home/root/trik/\"")
			.arg(qReal::SettingsManager::value("WinScpPath").toString())
			.arg(qReal::SettingsManager::value("TrikTcpServer").toString())
			.arg(fileInfo.absoluteFilePath().replace("pas", "exe").replace("/", "\\"));

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

void TrikPascalABCGeneratorPluginBase::runProgram()
{
	/// @todo: Implement this.
}

void TrikPascalABCGeneratorPluginBase::stopRobot()
{
	/// @todo: Implement this.
}
