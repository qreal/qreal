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

#include <thirdparty/qslog/QsLog.h>

#include "kernelDeclSpec.h"

namespace qReal {

/// Incapsulates some operations related to data logging.
/// Logger usage examples:
///     QLOG_INFO() << "Hi there from" << name;
///     QLOG_ERROR() << "An error has occurred"
class QRKERNEL_EXPORT Logger
{
public:
	/// Creates new logging target file: the logged info will be multiplexed into all created
	/// by this method target.
	/// @param path Absolute or relative path to new logging target.
	/// @param maxSize Maximal log file size in bytes. Logs are rotated: when the given size is exceeded and
	///        log files count is already equal to the next parameter then the old contents will be overwritten.
	/// @param maxOldLogsCount The maximal number of log files related to this target.
	/// @param level The minimal log entries level that will be accepter by this target.
	static void addLogTarget(const QString &path, int maxSize, int maxOldLogsCount
			, QsLogging::Level = QsLogging::InfoLevel);
};

}
