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

#pragma once

#include <trikGeneratorBase/trikGeneratorPluginBase.h>
#include <trikGeneratorBase/robotModel/generatorModelExtensionInterface.h>
#include <utils/uploaderTool.h>
#include <utils/robotCommunication/tcpRobotCommunicator.h>
#include <utils/robotCommunication/stopRobotProtocol.h>

namespace utils {
class TcpRobotCommunicator;
}

namespace trik {
namespace pascalABC {

class TrikPascalABCAdditionalPreferences;

/// Generation of PascalABC program for TRIK, uploading and execution a program
/// Uses settings "TrikTcpServer" and "PascalABCPath".
class TrikPascalABCGeneratorPluginBase : public TrikGeneratorPluginBase
{
	Q_OBJECT

public:
	TrikPascalABCGeneratorPluginBase(kitBase::robotModel::RobotModelInterface * const robotModel
									 , kitBase::blocksBase::BlocksFactoryInterface * const blocksFactory
									 , const QStringList &pathsToTemplates);

	~TrikPascalABCGeneratorPluginBase() override;

	QList<qReal::ActionInfo> customActions() override;
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;
	QIcon iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const override;
	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;
	void init(const kitBase::KitPluginConfigurator &configurator) override;

protected:
	generatorBase::MasterGeneratorBase *masterGenerator() override;
	QString defaultFilePath(const QString &projectName) const override;
	qReal::text::LanguageInfo language() const override;
	QString generatorName() const override;

private slots:
	/// Generates, compiles and uploads compiled program to a robot. It can then be run by Mono.
	/// @returns binary file name if successful or empty string if not.
	QString uploadProgram();

	/// Runs currently opened program on a robot. Compiles and uploads it first.
	void runProgram();

	/// Tries to remotely abort program execution and stop robot.
	void stopRobot();

private:
	/// Action that launches code generator.
	/// Doesn't have ownership; may be disposed by GUI.
	QAction *mGenerateCodeAction = nullptr;

	/// Action that generates and uploads program on a robot.
	/// Doesn't have ownership; may be disposed by GUI.
	QAction *mUploadProgramAction = nullptr;

	/// Action that generates, compiles, uploads and runs program on a robot.
	/// Doesn't have ownership; may be disposed by GUI.
	QAction *mRunProgramAction = nullptr;

	/// Action that stops program execution and turns off motors.
	/// Doesn't have ownership; may be disposed by GUI.
	QAction *mStopRobotAction = nullptr;

	TrikPascalABCAdditionalPreferences *mAdditionalPreferences = nullptr;  //Transfers ownership
	bool mOwnsAdditionalPreferences = true;

	/// Paths on which generator searches for templates. Templates can override each other, those who are later in
	/// the list take preference.
	QStringList mPathsToTemplates;

	/// TrikSharp runtime uploader.
	UploaderTool mRuntimeUploaderTool;

	/// Communicator object used to send commands to robot.
	QScopedPointer<utils::robotCommunication::TcpRobotCommunicator> mCommunicator;

	/// Protocol that is used to stop robot.
	QScopedPointer<utils::robotCommunication::StopRobotProtocol> mStopRobotProtocol;
};

}
}
