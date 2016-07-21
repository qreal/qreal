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

#include <utils/robotCommunication/robotCommunicator.h>
#include <ev3GeneratorBase/ev3GeneratorPluginBase.h>

namespace ev3 {
namespace rbf {

class Ev3RbfGeneratorPlugin : public Ev3GeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "ev3.Ev3RbfGeneratorPlugin")

public:
	Ev3RbfGeneratorPlugin();

	QList<qReal::ActionInfo> customActions() override;
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;
	QIcon iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const override;
	int priority() const override;

protected:
	generatorBase::MasterGeneratorBase *masterGenerator() override;
	QString defaultFilePath(const QString &projectName) const override;
	qReal::text::LanguageInfo language() const override;
	QString generatorName() const override;

private slots:
	/// Generates and uploads script to a EV3 robot.
	/// @return Empty string if operation was unsuccessfull or path to uploaded file on the EV3 robot otherwise.
	QString uploadProgram();

	/// Generates, uploads and starts script on the EV3 robot.
	void runProgram();

	/// Stops curretly executing program on the EV3 robot;
	void stopRobot();

private:
	/// Function that checks installed JRE or not
	bool javaInstalled();

	bool copySystemFiles(const QString &destination);
	bool compile(const QFileInfo &lmsFile);
	/// @returns path to uploaded file on EV3 brick if it was uploaded successfully or empty string otherwise.
	QString upload(const QFileInfo &lmsFile);

	/// Action that launches code generator
	QAction *mGenerateCodeAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Action that generates and uploads program on a robot
	QAction *mUploadProgramAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Action that generates, uploads and starts program on a robot
	QAction *mRunProgramAction;  // Doesn't have ownership; may be disposed by GUI.

	/// Action that stops current program on a robot
	QAction *mStopRobotAction;  // Doesn't have ownership; may be disposed by GUI.
};

}
}
