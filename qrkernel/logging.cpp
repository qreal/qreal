#include "logging.h"

using namespace qReal;

void Logger::addLogTarget(QString const &path, int maxSize, int maxOldLogsCount, QsLogging::Level level)
{
	QsLogging::DestinationPtr destination(QsLogging::DestinationFactory::MakeFileDestination(path
			, QsLogging::EnableLogRotation
			, QsLogging::MaxSizeBytes(maxSize)
			, QsLogging::MaxOldLogCount(maxOldLogsCount)
			, level));
	QsLogging::Logger::instance().setLoggingLevel(QsLogging::TraceLevel);
	QsLogging::Logger::instance().addDestination(destination);
}
