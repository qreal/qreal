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

#include <trikGeneratorBase/trikGeneratorPluginBase.h>

#include "declSpec.h"

namespace qReal {
class ErrorReporterInterface;
}

namespace utils {
namespace robotCommunication {

class TcpRobotCommunicator;
class StopRobotProtocol;

}
}

namespace trik {
namespace fSharp {

class TrikFSharpAdditionalPreferences;

/// Generation of F# program for TRIK, uploading and execution of a program.
/// Uses setting "tcpServer" from RobotsInterpreter.
class ROBOTS_TRIK_FSHARP_GENERATOR_LIBRARY_EXPORT TrikFSharpGeneratorPluginBase : public TrikGeneratorPluginBase
{
	Q_OBJECT

public:
	TrikFSharpGeneratorPluginBase(kitBase::robotModel::RobotModelInterface * const robotModel
				, kitBase::blocksBase::BlocksFactoryInterface * const blocksFactory
				, const QStringList &pathsToTemplates);

	~TrikFSharpGeneratorPluginBase() override;

	void init(const kitBase::KitPluginConfigurator &configurator) override;

	QList<qReal::ActionInfo> customActions() override;
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;
	QIcon iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const override;
	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;

protected:
	generatorBase::MasterGeneratorBase *masterGenerator() override;
	QString defaultFilePath(const QString &projectName) const override;
	qReal::text::LanguageInfo language() const override;
	QString generatorName() const override;

private slots:

	/// Generates and uploads script to a robot. Program then can be launched manually or remotely
	/// by runCommand. Program is stored on robot as a file in /home/root/trik/scripts and named
	/// as <qReal save name>.fs.
	/// @returns True, if successful.
	bool uploadProgram();

	/// Runs currently opened program on a robot. Uploads it first.
	void runProgram();

	/// Tries to remotely abort script execution and stop robot.
	void stopRobot();

private:
	/// Action that launches code generator
	QAction *mGenerateCodeAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Action that generates and uploads program on a robot
	QAction *mUploadProgramAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Action that generates and uploads program on a robot
	QAction *mRunProgramAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Action that stops script execution and turns off motors.
	QAction *mStopRobotAction;  // Doesn't have ownership; may be disposed by GUI.

	TrikFSharpAdditionalPreferences *mAdditionalPreferences = nullptr;  //Transfers ownership
	bool mOwnsAdditionalPreferences = true;

	const QStringList mPathsToTemplates;

	/// Communicator object used to send commands to robot.
	QScopedPointer<utils::robotCommunication::TcpRobotCommunicator> mCommunicator;

	/// Protocol that is used to stop robot.
	QScopedPointer<utils::robotCommunication::StopRobotProtocol> mStopRobotProtocol;
};

}
}
