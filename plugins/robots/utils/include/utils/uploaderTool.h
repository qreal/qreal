/* Copyright 2007-2015 QReal Research Group, CyberTech Labs Ltd.
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

#include <QtCore/QProcess>
#include <functional>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/actionInfo.h>

#include "utilsDeclSpec.h"

namespace trik {

/// Class that incapsulates SCP connection to upload something and/or execute commands on a robot.
/// Gets commands that need to be executed and produces action for toolbar that launches WinSCP, connects to a robot
/// and executes given commands.
class ROBOTS_UTILS_EXPORT UploaderTool : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param actionName - name of an action on toolbar.
	/// @param icon - icon for an action on toolbar.
	/// @param kit - id of a kit for which is this action. When other robot kit is selected action will hide itself.
	/// @param commands - commands to be executed on a robot when connection is established.
	/// @param startedMessage - message that will be shown in ErrorReporter when execution of commands is started.
	/// @param robotIpGetter - function that returns current robot ip address.
	UploaderTool(
			const QString &actionName
			, const QString &icon
			, const QString &kit
			, const QStringList &commands
			, const QString &startedMessage
			, const std::function<QString()> robotIpGetter
			);
	~UploaderTool();

	/// Initializes uploader. Must be called before action is triggered.
	/// @param path - path to directory to upload to robot
	void init(qReal::gui::MainWindowInterpretersInterface &mainWindowInterface, const QString &path);

	/// Returns an action that will trigger uploading/commands execution.
	/// Transfers ownership of QAction objects.
	qReal::ActionInfo action() const;

private slots:
	void uploadRuntime();
	void onUploadStarted();
	void onUploadError(QProcess::ProcessError reason);
	void onUploadFinished(int exitCode);
	void onUploadStdOut();
	void onUploadStdErr();

private:
	bool checkUnixToolsExist();
	bool checkUnixToolExist(const QString &name, const QStringList &args);

	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership
	QAction *mAction;  // Doesn't have ownership; may be disposed by GUI.
	QStringList mCommands;
	QString mStartedMessage;
	std::function<QString()> mRobotIpGetter;
	QProcess mProcess;
	QString mPath;
};

}
