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

#include <qrkernel/logging.h>
#include <qrkernel/platformInfo.h>
#include <qrkernel/settingsManager.h>
#include <qrkernel/settingsListener.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

using namespace trik;

UploaderTool::UploaderTool(
		const QString &actionName
		, const QString &icon
		, const QString &kit
		, const QStringList &commands
		, const QString &startedMessage
		, const std::function<QString()> robotIpGetter
		)
	: mMainWindowInterface(nullptr)
	, mAction(new QAction(QIcon(icon), actionName, nullptr))
	, mCommands(commands)
	, mStartedMessage(startedMessage)
	, mRobotIpGetter(robotIpGetter)
{
	connect(mAction, &QAction::triggered, this, &UploaderTool::uploadRuntime);
	mAction->setVisible(qReal::SettingsManager::value("SelectedRobotKit").toString() == kit);
	qReal::SettingsListener::listen("SelectedRobotKit", [this, kit](const QString selectedKit) {
		mAction->setVisible(selectedKit == kit);
	}, this);

	connect(&mProcess, &QProcess::started, this, &UploaderTool::onUploadStarted);
	connect(&mProcess, static_cast<void(QProcess::*)(QProcess::ProcessError)>(&QProcess::error)
			, this, &UploaderTool::onUploadError);
	connect(&mProcess, static_cast<void(QProcess::*)(int)>(&QProcess::finished)
			, this, &UploaderTool::onUploadFinished);
	connect(&mProcess, &QProcess::readyReadStandardOutput, this, &UploaderTool::onUploadStdOut);
	connect(&mProcess, &QProcess::readyReadStandardError, this, &UploaderTool::onUploadStdErr);
}

UploaderTool::~UploaderTool()
{
	mMainWindowInterface = nullptr;
	disconnect(&mProcess);
	disconnect(this);
	mProcess.terminate();
}

void UploaderTool::init(qReal::gui::MainWindowInterpretersInterface &mainWindowInterface, const QString &path)
{
	mPath = path;
	mProcess.setWorkingDirectory(mPath);
	mMainWindowInterface = &mainWindowInterface;
}

qReal::ActionInfo UploaderTool::action() const
{
	return qReal::ActionInfo(mAction, "", "tools");
}

void UploaderTool::uploadRuntime()
{
	if (mProcess.state() != QProcess::NotRunning) {
		QLOG_WARN() << "Attempted to upload during uploading!";
		if (mMainWindowInterface) {
			mMainWindowInterface->errorReporter()->addInformation("Uploading is already running.");
		}

		return;
	}

#ifdef Q_OS_WIN
	const QString openConnection = QString("open scp://root@%1 -hostkey=*").arg(mRobotIpGetter());
	const QString uploaderPath = qReal::PlatformInfo::invariantSettingsPath("WinScpPath");
	QStringList args = {"/command", openConnection};
	args << mCommands;
	args << "exit";
#else
	if (!checkUnixToolsExist()) {
		return;
	}

	const QString uploaderPath = "bash";
	QStringList actions;
	for (QString command /* Not by const reference cause it will be modified next line */ : mCommands) {
		actions << command
				.replace("%IP%", mRobotIpGetter())
				.replace("%PATH%", mPath)
				.replace("%SSH_TIMEOUT%", qReal::SettingsManager::value("sshTimeout").toString());
	}

	const QStringList args = { "-x", "-c", actions.join("; ") };
#endif

	QLOG_INFO() << "TRIK Runtime uploading is about to start. Path:" << uploaderPath << "Args: " << args;
	mProcess.start(uploaderPath, args);
}

void UploaderTool::onUploadStarted()
{
	QLOG_INFO() << "TRIK Runtime uploading process started successfully...";
	if (mMainWindowInterface) {
		mMainWindowInterface->errorReporter()->addWarning(mStartedMessage);
	}
}

void UploaderTool::onUploadError(QProcess::ProcessError reason)
{
	if (!mMainWindowInterface) {
		return;
	}

	if (reason == QProcess::FailedToStart) {
		QLOG_ERROR() << "TRIK Runtime uploading process failed to start! Details:" << mProcess.errorString();
		mMainWindowInterface->errorReporter()->addError(tr("WinSCP process failed to launch, check path in settings."));
	} else {
		QLOG_ERROR() << "TRIK Runtime uploading process failed! Details:" << mProcess.errorString();
		mMainWindowInterface->errorReporter()->addError(tr("Uploading failed, check connection and try again."));
	}
}

void UploaderTool::onUploadFinished(int exitCode)
{
	if (!mMainWindowInterface) {
		return;
	}

	if (exitCode == 0) {
		QLOG_INFO() << "TRIK Runtime uploading process successfully finished.";
		mMainWindowInterface->errorReporter()->addInformation(tr("Uploaded successfully!"));
	} else {
		QLOG_ERROR() << "TRIK Runtime uploading process failed with exit code" << exitCode;
		mMainWindowInterface->errorReporter()->addError(tr("Uploading failed, check connection and try again."));
	}
}

void UploaderTool::onUploadStdOut()
{
	QLOG_DEBUG() << mProcess.readAllStandardOutput();
}

void UploaderTool::onUploadStdErr()
{
	QLOG_DEBUG() << mProcess.readAllStandardError();
}

bool UploaderTool::checkUnixToolsExist()
{
	return checkUnixToolExist("ssh", { "-V" }) && checkUnixToolExist("scp", {});
}

bool UploaderTool::checkUnixToolExist(const QString &name, const QStringList &args)
{
	QLOG_DEBUG() << "Starting" << qPrintable(name) << args;
	QProcess tool;
	connect(&tool, &QProcess::readyReadStandardOutput, [&tool]() { QLOG_DEBUG() << tool.readAllStandardOutput(); });
	connect(&tool, &QProcess::readyReadStandardError, [&tool]() { QLOG_DEBUG() << tool.readAllStandardError(); });
	tool.start(name, args);
	if (!tool.waitForFinished()) {
		QLOG_ERROR() << qPrintable(name) << "failed to start; error: " << tool.errorString();
		mMainWindowInterface->errorReporter()->addError(tr("%1 is not installed. Please install %1 first.").arg(name));
		return false;
	}

	return true;
}
