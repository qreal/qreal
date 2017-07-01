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

class QFileInfo;
class QNetworkAccessManager;
class QNetworkReply;
class QProcess;
class QTimer;

namespace qReal {
class ErrorReporterInterface;
}

namespace pioneer {
namespace lua {

/// Communicates with quadcopter using HTTP POST requests.
class HttpCommunicator : public CommunicatorInterface
{
	Q_OBJECT

public:
	HttpCommunicator(qReal::ErrorReporterInterface &errorReporter);

	~HttpCommunicator() override;

	void uploadProgram(const QFileInfo &program) override;

	void runProgram() override;

	void stopProgram() override;

private slots:
	/// Called when network POST request is finished.
	void onPostRequestFinished(QNetworkReply *reply);

	/// Called when connection is established but there is no response from a quadcopter.
	void onTimeout();

private:
	/// Manager that is used to communicate with base station over HTTP protocol.
	QScopedPointer<QNetworkAccessManager> mNetworkManager;

	/// Error reporter that shows messages and errors in TRIK Studio main window.
	/// Does not have ownership.
	qReal::ErrorReporterInterface &mErrorReporter;

	/// Provides error report when quadcopter does not respond in time.
	QScopedPointer<QTimer> mRequestTimeoutTimer;

	/// Currently active request (nullptr most of the time). Used to abort request on timeout.
	/// Does not have ownership.
	QNetworkReply *mCurrentReply;
};

}
}
