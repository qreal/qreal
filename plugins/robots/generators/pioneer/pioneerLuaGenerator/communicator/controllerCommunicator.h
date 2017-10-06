/* Copyright 2017 QReal Research Group
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

#include "communicator/communicatorInterface.h"

class QProcess;
class QTimer;

namespace qReal {
class ErrorReporterInterface;
}

namespace kitBase {
namespace robotModel {
class RobotModelManagerInterface;
}
}

namespace pioneer {
namespace lua {

/// Communicates with quadcopter using "controller" stand-alone program.
class ControllerCommunicator : public CommunicatorInterface
{
	Q_OBJECT

public:
	ControllerCommunicator(
			qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
	);

	~ControllerCommunicator() override;

	void uploadProgram(const QFileInfo &program) override;

	void startProgram() override;

	void stopProgram() override;

private slots:
	/// Called by upload process when it is done.
	void onUploadCompleted();

	/// Called by program start process when it is done.
	void onStartCompleted();

	/// Called by program stop process when it is done.
	void onStopCompleted();

	/// Called when some process took too long to finish.
	void onTimeout();

private:
	/// Helper method that correctly converts given console output into unicode string.
	QString toUnicode(const QByteArray &str);

	/// Returns address to which requests for uploading or running program shall be sent. Respects COM/IP settings.
	/// Reports error and returns empty string if settings are incorrect.
	QString address();

	/// Sends process output and process error stream to error reporter.
	void reportOutput(QProcess &process);

	/// Process handler for uploading program.
	QScopedPointer<QProcess> mUploadProcess;

	/// Process handler for starting program.
	QScopedPointer<QProcess> mStartProcess;

	/// Process handler for stopping program.
	QScopedPointer<QProcess> mStopProcess;

	/// Error reporter that shows messages and errors in TRIK Studio main window.
	/// Does not have ownership.
	qReal::ErrorReporterInterface &mErrorReporter;

	/// Provides information about currently selected robot model.
	const kitBase::robotModel::RobotModelManagerInterface &mRobotModelManager;

	/// Keeps Upload operation from taking too long, killing the process if needed.
	QScopedPointer<QTimer> mUploadTimeoutTimer;

	/// Keeps Start operation from taking too long, killing the process if needed.
	QScopedPointer<QTimer> mStartTimeoutTimer;

	/// Keeps Stop operation from taking too long, killing the process if needed.
	QScopedPointer<QTimer> mStopTimeoutTimer;
};

}
}
