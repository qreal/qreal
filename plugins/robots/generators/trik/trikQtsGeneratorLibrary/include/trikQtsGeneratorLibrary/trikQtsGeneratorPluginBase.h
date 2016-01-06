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

#pragma once

#include <QtCore/QScopedPointer>

#include <trikGeneratorBase/trikGeneratorPluginBase.h>
#include <trikGeneratorBase/robotModel/generatorModelExtensionInterface.h>

namespace qReal {
class ErrorReporterInterface;
}

namespace utils {
namespace robotCommunication {

class TcpRobotCommunicator;
class RunProgramProtocol;
class StopRobotProtocol;
class UploadProgramProtocol;

}
}

namespace trik {
namespace qts {

/// Generation of QtScript program for TRIK, uploading and execution of a program.
/// Uses setting "tcpServer" from RobotsInterpreter.
class TrikQtsGeneratorPluginBase : public TrikGeneratorPluginBase
{
	Q_OBJECT

public:
	TrikQtsGeneratorPluginBase(kitBase::robotModel::RobotModelInterface * const robotModel
			, kitBase::blocksBase::BlocksFactoryInterface * const blocksFactory
			, const QStringList &pathsToTemplates);

	~TrikQtsGeneratorPluginBase() override;

	void init(const kitBase::KitPluginConfigurator &configurator) override;

	QList<qReal::ActionInfo> customActions() override;
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;
	QIcon iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const override;

protected:
	generatorBase::MasterGeneratorBase *masterGenerator() override;
	QString defaultFilePath(const QString &projectName) const override;
	qReal::text::LanguageInfo language() const override;
	QString generatorName() const override;

	void addShellDevice(robotModel::GeneratorModelExtensionInterface &robotModel) const;

private slots:
	/// Generates and uploads script to a robot. Program then can be launched manually or remotely
	/// by runCommand. Program is stored on robot as a file next to scriptRunner and named
	/// as <qReal save name>.qts.
	void uploadProgram();

	/// Runs currently opened program on a robot. Uploads it first.
	void runProgram();

	/// Tries to remotely abort script execution and stop robot.
	void stopRobot();

	/// Reenables buttons when program uploading or robot stopping is complete (sucessfully or with error).
	void onProtocolFinished();

private:
	/// Disables "run", "upload" and "stop" buttons when there is pending command to a robot.
	void disableButtons();

	/// Action that launches code generator
	QAction *mGenerateCodeAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Action that generates and uploads program on a robot
	QAction *mUploadProgramAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Action that generates and uploads program on a robot
	QAction *mRunProgramAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Action that stops script execution and turns off motors.
	QAction *mStopRobotAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Communicator object used to send commands to robot.
	QScopedPointer<utils::robotCommunication::TcpRobotCommunicator> mCommunicator;

	QStringList mPathsToTemplates;

	/// Protocol that is used to upload program to robot.
	QScopedPointer<utils::robotCommunication::UploadProgramProtocol> mUploadProgramProtocol;

	/// Protocol that is used to upload and run program.
	QScopedPointer<utils::robotCommunication::RunProgramProtocol> mRunProgramProtocol;

	/// Protocol that is used to stop robot.
	QScopedPointer<utils::robotCommunication::StopRobotProtocol> mStopRobotProtocol;
};

}
}
