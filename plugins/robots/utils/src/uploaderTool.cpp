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

#include "utils/uploaderTool.h"

#include <QtCore/QProcess>
#include <QtWidgets/QApplication>

#include <qrkernel/settingsManager.h>
#include <qrkernel/settingsListener.h>
#include <qrkernel/platformInfo.h>

using namespace trik;

UploaderTool::UploaderTool(
		const QString &actionName
		, const QString &icon
		, const QString &kit
		, const QStringList &commands
		, const QString &startedMessage
		, const std::function<QString()> robotIpGetter
		)
	: mAction(new QAction(QIcon(icon), actionName, nullptr))
	, mCommands(commands)
	, mStartedMessage(startedMessage)
	, mRobotIpGetter(robotIpGetter)
{
	connect(mAction, &QAction::triggered, this, &UploaderTool::uploadRuntime);
	mAction->setVisible(qReal::SettingsManager::value("SelectedRobotKit").toString() == kit);
	qReal::SettingsListener::listen("SelectedRobotKit", [this, kit](const QString selectedKit) {
		mAction->setVisible(selectedKit == kit);
	});
}

void UploaderTool::init(qReal::gui::MainWindowInterpretersInterface &mainWindowInterface)
{
	mMainWindowInterface = &mainWindowInterface;
}

qReal::ActionInfo UploaderTool::action() const
{
	return qReal::ActionInfo(mAction, "", "tools");
}

void UploaderTool::uploadRuntime()
{
	const QString openConnection = QString("open scp://root@%1").arg(mRobotIpGetter());

	const QString rawWinscpPath = qReal::SettingsManager::value("WinScpPath").toString();
	const QString winscpPath = rawWinscpPath.startsWith("./")
			? qReal::PlatformInfo::applicationDirPath() + rawWinscpPath.mid(1)
			: rawWinscpPath;

	QStringList args = {"/command", openConnection};

	args << mCommands;

	args << "exit";

	if (!QProcess::startDetached(winscpPath, args, qReal::PlatformInfo::applicationDirPath())) {
		mMainWindowInterface->errorReporter()->addError(
			tr("WinSCP process failed to launch, check path in settings.")
		);
	} else {
		mMainWindowInterface->errorReporter()->addWarning(mStartedMessage);
	}
}
