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

#include "trikQtsGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include <trikGeneratorBase/trikGeneratorPluginBase.h>
#include <trikGeneratorBase/robotModel/trikGeneratorRobotModel.h>
#include <utils/tcpRobotCommunicator.h>

#include "trikQtsMasterGenerator.h"
#include "emptyShell.h"

using namespace trik::qts;
using namespace kitBase::robotModel;
using namespace qReal;

TrikQtsGeneratorPlugin::TrikQtsGeneratorPlugin()
	: TrikGeneratorPluginBase("TrikQtsGeneratorRobotModel", tr("Generation (QtScript)"), 9 /* After 2D model */)
	, mGenerateCodeAction(new QAction(nullptr))
	, mUploadProgramAction(new QAction(nullptr))
	, mRunProgramAction(new QAction(nullptr))
	, mStopRobotAction(new QAction(nullptr))
	, mCommunicator(nullptr)
{
}

TrikQtsGeneratorPlugin::~TrikQtsGeneratorPlugin()
{
	delete mCommunicator;
}

void TrikQtsGeneratorPlugin::init(const kitBase::KitPluginConfigurator &configurator)
{
	RobotsGeneratorPluginBase::init(configurator);
	mCommunicator = new utils::TcpRobotCommunicator("TrikTcpServer");
	mCommunicator->setErrorReporter(configurator.qRealConfigurator().mainWindowInterpretersInterface().errorReporter());
	const PortInfo shellPort("ShellPort", output);
	EmptyShell * const shell = new EmptyShell(DeviceInfo::create<trik::robotModel::parts::TrikShell>(), shellPort);
	connect(mCommunicator, &utils::TcpRobotCommunicator::printText, shell, &EmptyShell::print);
	mRobotModel->addDevice(shellPort, shell);
}

QList<ActionInfo> TrikQtsGeneratorPlugin::customActions()
{
	mGenerateCodeAction->setText(tr("Generate TRIK code"));
	mGenerateCodeAction->setIcon(QIcon(":/trik/qts/images/generateQtsCode.svg"));
	ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	connect(mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()), Qt::UniqueConnection);

	mUploadProgramAction->setText(tr("Upload program"));
	mUploadProgramAction->setIcon(QIcon(":/trik/qts/images/uploadProgram.svg"));
	ActionInfo uploadProgramActionInfo(mUploadProgramAction, "generators", "tools");
	connect(mUploadProgramAction, SIGNAL(triggered()), this, SLOT(uploadProgram()), Qt::UniqueConnection);

	mRunProgramAction->setText(tr("Run program"));
	mRunProgramAction->setIcon(QIcon(":/trik/qts/images/run.png"));
	ActionInfo runProgramActionInfo(mRunProgramAction, "interpreters", "tools");
	connect(mRunProgramAction, SIGNAL(triggered()), this, SLOT(runProgram()), Qt::UniqueConnection);

	mStopRobotAction->setText(tr("Stop robot"));
	mStopRobotAction->setIcon(QIcon(":/trik/qts/images/stop.png"));
	ActionInfo stopRobotActionInfo(mStopRobotAction, "interpreters", "tools");
	connect(mStopRobotAction, SIGNAL(triggered()), this, SLOT(stopRobot()), Qt::UniqueConnection);

	return {generateCodeActionInfo, uploadProgramActionInfo, runProgramActionInfo, stopRobotActionInfo};
}

QList<HotKeyActionInfo> TrikQtsGeneratorPlugin::hotKeyActions()
{
	mGenerateCodeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
	mUploadProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
	mRunProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
	mStopRobotAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F5));

	HotKeyActionInfo generateCodeInfo("Generator.GenerateTrik", tr("Generate TRIK Code"), mGenerateCodeAction);
	HotKeyActionInfo uploadProgramInfo("Generator.UploadTrik", tr("Upload TRIK Program"), mUploadProgramAction);
	HotKeyActionInfo runProgramInfo("Generator.RunTrik", tr("Run TRIK Program"), mRunProgramAction);
	HotKeyActionInfo stopRobotInfo("Generator.StopTrik", tr("Stop TRIK Robot"), mStopRobotAction);

	return { generateCodeInfo, uploadProgramInfo, runProgramInfo, stopRobotInfo };
}

QIcon TrikQtsGeneratorPlugin::iconForFastSelector(const RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon(":/trik/qts/images/switch-to-trik-qts.svg");
}

generatorBase::MasterGeneratorBase *TrikQtsGeneratorPlugin::masterGenerator()
{
	return new TrikQtsMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mParserErrorReporter
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, generatorName());
}

QString TrikQtsGeneratorPlugin::defaultFilePath(const QString &projectName) const
{
	return QString("trik/%1/%1.qts").arg(projectName);
}

text::LanguageInfo TrikQtsGeneratorPlugin::language() const
{
	return qReal::text::Languages::qtScript({ "brick" });
}

QString TrikQtsGeneratorPlugin::generatorName() const
{
	return "trikQts";
}

bool TrikQtsGeneratorPlugin::uploadProgram()
{
	const QFileInfo fileInfo = generateCodeForProcessing();

	if (fileInfo != QFileInfo() && !fileInfo.absoluteFilePath().isEmpty()) {
		const bool result = mCommunicator->uploadProgram(fileInfo.absoluteFilePath());
		if (!result) {
			mMainWindowInterface->errorReporter()->addError(tr("No connection to robot"));
		}

		return result;
	} else {
		qDebug() << "Code generation failed, aborting";
		return false;
	}
}

void TrikQtsGeneratorPlugin::runProgram()
{
	if (uploadProgram()) {
		const QFileInfo fileInfo = generateCodeForProcessing();
		mCommunicator->runProgram(fileInfo.fileName());
	} else {
		qDebug() << "Program upload failed, aborting";
	}
}

void TrikQtsGeneratorPlugin::stopRobot()
{
	if (!mCommunicator->stopRobot()) {
		mMainWindowInterface->errorReporter()->addError(tr("No connection to robot"));
	}

	mCommunicator->runDirectCommand(
			"script.system(\"killall aplay\"); \n"
			"script.system(\"killall vlc\");"
			, true
			);
}
