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

#include <QtCore/QFileInfo>

#include "communicator/communicatorInterface.h"

class QProcess;
class QNetworkAccessManager;
class QNetworkReply;

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

/// Communicates with quadcopter using HTTP POST requests.
class HttpCommunicator : public CommunicatorInterface
{
	Q_OBJECT

public:
	HttpCommunicator(
			qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
	);

	~HttpCommunicator() override;

	void uploadProgram(const QFileInfo &program) override;

	void runProgram(const QFileInfo &program) override;

private slots:
	/// Called when compilation process is finished.
	void onCompilationDone();

	/// Called when network POST request is finished.
	void onPostRequestFinished(QNetworkReply *reply);

private:
	/// Mark current procerss as done, emitting apropriate signal.
	void done();

	/// Initiates "start" request.
	void doRunProgram();

	/// Process handler for compiling program. We use shell script for compilation to make compiler call more
	/// configurable and to use things like PATH and other shell features.
	QScopedPointer<QProcess> mCompileProcess;

	/// Manager that is used to communicate with base station over HTTP protocol.
	QScopedPointer<QNetworkAccessManager> mNetworkManager;

	/// Error reporter that shows messages and errors in TRIK Studio main window.
	/// Does not have ownership.
	qReal::ErrorReporterInterface &mErrorReporter;

	/// Provides information about currently selected robot model.
	const kitBase::robotModel::RobotModelManagerInterface &mRobotModelManager;

	/// State of an asynchronous operation --- do we need to start a program after uploading or not.
	bool mIsStartNeeded = false;

	/// Information about current program location. Is set every time when upload or run request begins to execute.
	QFileInfo mCurrentProgram;
};

}
}
