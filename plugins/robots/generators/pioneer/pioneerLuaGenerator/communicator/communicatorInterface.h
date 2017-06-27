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

class QFileInfo;

namespace qReal {
class ErrorReporterInterface;
}

namespace pioneer {
namespace lua {

/// Interface that declares the abilities to asynchronously upload and run program on a quadcopter.
class CommunicatorInterface : public QObject
{
	Q_OBJECT

public:
	/// Virtual destructor, because C++ is horrible.
	virtual ~CommunicatorInterface() {}

	/// Uploads current program to a quadcopter.
	virtual void uploadProgram(const QFileInfo &program) = 0;

	/// Attempts to run current program on a quadcopter. Generates and uploads it first.
	virtual void runProgram(const QFileInfo &program) = 0;

	/// Attempts to stop currently executed program.
	virtual void stopProgram() = 0;

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
};

}
}
