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

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>

class QFileInfo;

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

class CommunicatorInterface;

/// Communicates with quadcopter using and coordinating underlying communicator.
class CommunicationManager : public QObject
{
	Q_OBJECT

public:
	CommunicationManager(
			qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
	);

	~CommunicationManager() override;

	void uploadProgram(const QFileInfo &program);

	void runProgram(const QFileInfo &program);

	void stopProgram();

signals:
	/// Emitted when upload is finished (successfully or with errors). Is not emitted when uploading is a part of
	/// running the program. uploadCompleted() is guaranteed to be sent after uploadProgram() call.
	void uploadCompleted();

	/// Emitted when "run" command is sent to a quadcopter (successfully or with errors). Note that program
	/// continues to execute on a quadcopter after this signal is emitted. runCompleted() is guaranteed to be sent
	/// after runProgram() call.
	void runCompleted();

	/// Emitted when "stop" command is sent to a quadcopter (successfully or with errors). stopCompleted() is
	/// guaranteed to be sent after stopProgram() call.
	void stopCompleted();

private slots:
	/// Called by upload process when it is done.
	void onUploadCompleted(bool isSuccessful);

	/// Called by program start process when it is done.
	void onStartCompleted(bool isSuccessful);

	/// Called by program stop process when it is done.
	void onStopCompleted(bool isSuccessful);

private:
	/// Enum with possible actions of communicator.
	enum class Action {
		none
		, uploading
		, starting
		, stopping
	};

	/// Returns reference to a currently selected quadcopter communicator.
	CommunicatorInterface &communicator() const;

	/// Mark current procerss as done, emitting appropriate signal.
	void done();

	/// Current action of a communicator.
	Action mCurrentAction = Action::none;

	/// Communicator object that communicates with robot using HTTP requests.
	QScopedPointer<CommunicatorInterface> mHttpCommunicator;
};

}
}
