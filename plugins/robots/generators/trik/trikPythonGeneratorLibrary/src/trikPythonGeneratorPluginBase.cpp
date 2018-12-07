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

#include "trikPythonGeneratorLibrary/trikPythonGeneratorPluginBase.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QStateMachine>
#include <QtCore/QState>
#include <QtCore/QFinalState>

#include <qrkernel/logging.h>
#include <trikGeneratorBase/trikGeneratorPluginBase.h>
#include <trikGeneratorBase/robotModel/generatorModelExtensionInterface.h>
#include <trikKit/robotModel/trikRobotModelBase.h>
#include <utils/robotCommunication/tcpRobotCommunicator.h>
#include <utils/robotCommunication/runProgramProtocol.h>
#include <utils/robotCommunication/stopRobotProtocol.h>
#include <utils/robotCommunication/uploadProgramProtocol.h>
#include <utils/robotCommunication/networkCommunicationErrorReporter.h>

#include "trikPythonMasterGenerator.h"
#include "emptyShell.h"

using namespace trik::python;
using namespace kitBase::robotModel;
using namespace qReal;
using namespace utils::robotCommunication;

TrikPythonGeneratorPluginBase::TrikPythonGeneratorPluginBase(
		trik::robotModel::TrikRobotModelBase * const robotModel
		, kitBase::blocksBase::BlocksFactoryInterface * const blocksFactory
		, const QStringList &pathsToTemplates)
	: TrikGeneratorPluginBase(robotModel, blocksFactory)
	, mGenerateCodeAction(new QAction(nullptr))
	, mUploadProgramAction(new QAction(nullptr))
	, mRunProgramAction(new QAction(nullptr))
	, mStopRobotAction(new QAction(nullptr))
	, mRobotModel(*robotModel)
	, mPathsToTemplates(pathsToTemplates)
{
}

TrikPythonGeneratorPluginBase::~TrikPythonGeneratorPluginBase()
{
}

void TrikPythonGeneratorPluginBase::init(const kitBase::KitPluginConfigurator &configurer)
{
	const auto errorReporter = configurer.qRealConfigurator().mainWindowInterpretersInterface().errorReporter();
	RobotsGeneratorPluginBase::init(configurer);
	mCommunicator.reset(new TcpRobotCommunicator("TrikTcpServer"));
	NetworkCommunicationErrorReporter::connectErrorReporter(*mCommunicator, *errorReporter);

	mUploadProgramProtocol.reset(new UploadProgramProtocol(*mCommunicator));
	mRunProgramProtocol.reset(new RunProgramProtocol(*mCommunicator, mRobotModel.robotConfigFileVersion()));
	mStopRobotProtocol.reset(new StopRobotProtocol(*mCommunicator));

	const auto timeout = [this, errorReporter]() {
		errorReporter->addError(tr("Network operation timed out"));
		onProtocolFinished();
	};

	connect(mUploadProgramProtocol.data(), &UploadProgramProtocol::timeout, this, timeout);
	connect(mRunProgramProtocol.data(), &RunProgramProtocol::timeout, this, timeout);
	connect(mStopRobotProtocol.data(), &StopRobotProtocol::timeout, this, timeout);

	connect(mUploadProgramProtocol.data(), &UploadProgramProtocol::error
			, this, &TrikPythonGeneratorPluginBase::onProtocolFinished);
	connect(mRunProgramProtocol.data(), &RunProgramProtocol::error
			, this, &TrikPythonGeneratorPluginBase::onProtocolFinished);
	connect(mStopRobotProtocol.data(), &StopRobotProtocol::error
			, this, &TrikPythonGeneratorPluginBase::onProtocolFinished);

	connect(mUploadProgramProtocol.data(), &UploadProgramProtocol::success
			, this, &TrikPythonGeneratorPluginBase::onProtocolFinished);
	connect(mRunProgramProtocol.data(), &RunProgramProtocol::success
			, this, &TrikPythonGeneratorPluginBase::onProtocolFinished);
	connect(mStopRobotProtocol.data(), &StopRobotProtocol::success
			, this, &TrikPythonGeneratorPluginBase::onProtocolFinished);

	connect(mRunProgramProtocol.data(), &RunProgramProtocol::configVersionMismatch
			, this, [errorReporter](const QString &expected, const QString &actual) {
				Q_UNUSED(expected)
				Q_UNUSED(actual)
				errorReporter->addError(
						QString(tr("Casing model mismatch, check TRIK Studio settings, \"Robots\" page. It seems that "
								"TRIK casing version selected in TRIK Studio differs from version on robot.")));
			}
	);
}

QList<ActionInfo> TrikPythonGeneratorPluginBase::customActions()
{
	mGenerateCodeAction->setObjectName("generatePythonTrikCode");
	mGenerateCodeAction->setText(tr("Generate python code"));
	mGenerateCodeAction->setIcon(QIcon(":/trik/python/images/generatePythonCode.svg"));
	ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	connect(mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()), Qt::UniqueConnection);

	mUploadProgramAction->setObjectName("uploadPythonTrikProgram");
	mUploadProgramAction->setText(tr("Upload program"));
	mUploadProgramAction->setIcon(QIcon(":/trik/python/images/uploadProgram.svg"));
	ActionInfo uploadProgramActionInfo(mUploadProgramAction, "generators", "tools");
	connect(mUploadProgramAction, SIGNAL(triggered()), this, SLOT(uploadProgram()), Qt::UniqueConnection);

	mRunProgramAction->setObjectName("runPythonTrikProgram");
	mRunProgramAction->setText(tr("Run program"));
	mRunProgramAction->setIcon(QIcon(":/trik/python/images/run.png"));
	ActionInfo runProgramActionInfo(mRunProgramAction, "interpreters", "tools");
	connect(mRunProgramAction, SIGNAL(triggered()), this, SLOT(runProgram()), Qt::UniqueConnection);

	mStopRobotAction->setObjectName("stopPythonTrikRobot");
	mStopRobotAction->setText(tr("Stop robot"));
	mStopRobotAction->setIcon(QIcon(":/trik/python/images/stop.png"));
	ActionInfo stopRobotActionInfo(mStopRobotAction, "interpreters", "tools");
	connect(mStopRobotAction, SIGNAL(triggered()), this, SLOT(stopRobot()), Qt::UniqueConnection);

	return {generateCodeActionInfo, uploadProgramActionInfo, runProgramActionInfo, stopRobotActionInfo};
}

QList<HotKeyActionInfo> TrikPythonGeneratorPluginBase::hotKeyActions()
{
	mGenerateCodeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
	mUploadProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
	mRunProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
	mStopRobotAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F5));

	HotKeyActionInfo generateCodeInfo("Generator.GenerateTrik", tr("Generate Python code"), mGenerateCodeAction);
	HotKeyActionInfo uploadProgramInfo("Generator.UploadTrik", tr("Upload Python program"), mUploadProgramAction);
	HotKeyActionInfo runProgramInfo("Generator.RunTrik", tr("Run Python program"), mRunProgramAction);
	HotKeyActionInfo stopRobotInfo("Generator.StopTrik", tr("Stop Python program"), mStopRobotAction);

	return {generateCodeInfo, uploadProgramInfo, runProgramInfo, stopRobotInfo};
}

QIcon TrikPythonGeneratorPluginBase::iconForFastSelector(const RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)

	return QIcon(":/trik/python/images/switch-to-trik-python.svg");
}

generatorBase::MasterGeneratorBase *TrikPythonGeneratorPluginBase::masterGenerator()
{
	return new TrikPythonMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mParserErrorReporter
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, mPathsToTemplates);
}

QString TrikPythonGeneratorPluginBase::defaultFilePath(const QString &projectName) const
{
	return QString("trik/%1/%1.py").arg(projectName);
}

text::LanguageInfo TrikPythonGeneratorPluginBase::language() const
{
	return qReal::text::Languages::python({"brick"});
}

QString TrikPythonGeneratorPluginBase::generatorName() const
{
	return "trikPython";
}

void TrikPythonGeneratorPluginBase::addShellDevice(robotModel::GeneratorModelExtensionInterface &robotModel) const
{
	const PortInfo shellPort("ShellPort", output);
	EmptyShell * const shell = new EmptyShell(DeviceInfo::create<trik::robotModel::parts::TrikShell>(), shellPort);
	connect(mCommunicator.data(), &TcpRobotCommunicator::printText, shell, &EmptyShell::print);
	robotModel.addDevice(shellPort, shell);
}

void TrikPythonGeneratorPluginBase::uploadProgram()
{
	const QFileInfo fileInfo = generateCodeForProcessing();

	if (fileInfo != QFileInfo() && !fileInfo.absoluteFilePath().isEmpty()) {
		disableButtons();
		mUploadProgramProtocol->run(fileInfo);
	} else {
		QLOG_ERROR() << "Code generation failed, aborting";
	}
}

void TrikPythonGeneratorPluginBase::runProgram()
{
	const QFileInfo fileInfo = generateCodeForProcessing();
	if (fileInfo != QFileInfo() && !fileInfo.absoluteFilePath().isEmpty()) {
		if (mRunProgramProtocol) {
			disableButtons();
			mRunProgramProtocol->run(fileInfo);
		} else {
			QLOG_ERROR() << "Run program protocol is not initialized";
		}
	} else {
		QLOG_ERROR() << "Code generation failed, aborting";
	}
}

void TrikPythonGeneratorPluginBase::stopRobot()
{
	if (mStopRobotProtocol) {
		disableButtons();
		mStopRobotProtocol->run(
				"script.system(\"killall python\"); \n"
				"script.system(\"killall aplay\"); \n"
				"script.system(\"killall vlc\");"
				);
	} else {
		QLOG_ERROR() << "Stop robot protocol is not initialized";
	}
}

void TrikPythonGeneratorPluginBase::onProtocolFinished()
{
	mUploadProgramAction->setEnabled(true);
	mRunProgramAction->setEnabled(true);
	mStopRobotAction->setEnabled(true);
}

void TrikPythonGeneratorPluginBase::disableButtons()
{
	mUploadProgramAction->setEnabled(false);
	mRunProgramAction->setEnabled(false);
	mStopRobotAction->setEnabled(false);
}
