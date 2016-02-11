/* Copyright 2016 Yurii Litvinov
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

#include "utils/utilsDeclSpec.h"

namespace qReal {
class ErrorReporterInterface;
}

namespace utils {
namespace robotCommunication {

class TcpRobotCommunicator;

/// Helper static class that connects TcpRobotCommunicator to Error reporter, adding all network error messages.
class ROBOTS_UTILS_EXPORT NetworkCommunicationErrorReporter
{
public:
	/// No constructor is needed.
	NetworkCommunicationErrorReporter() = delete;

	/// Connects TcpRobotCommunicator to ErrorReporter, prints network errors from communicator.
	static void connectErrorReporter(const TcpRobotCommunicator &communicator
			, qReal::ErrorReporterInterface &errorReporter);
};

}
}
