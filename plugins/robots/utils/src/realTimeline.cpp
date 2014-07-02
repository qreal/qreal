#include "utils/realTimeline.h"

#include <QtCore/QDateTime>

#include "utils/realTimer.h"

using namespace utils;

quint64 RealTimeline::timestamp() const
{
	return QDateTime::currentMSecsSinceEpoch();
}

AbstractTimer *RealTimeline::produceTimer()
{
	return new RealTimer;
}
