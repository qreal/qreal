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
