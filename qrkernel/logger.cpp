#include "logger.h"

using namespace qReal;

void Logger::addLogTarget(QString const &path, int maxSize, int maxOldLogsCount)
{
	QsLogging::DestinationPtr destination(QsLogging::DestinationFactory::MakeFileDestination(path
			, QsLogging::EnableLogRotation
			, QsLogging::MaxSizeBytes(maxSize)
			, QsLogging::MaxOldLogCount(maxOldLogsCount)));
	QsLogging::Logger::instance().addDestination(destination);
}
