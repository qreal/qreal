#include "realTimeline.h"

#include <QtCore/QDateTime>

#include "realTimer.h"

using namespace qReal::interpreters::robots::details;

quint64 RealTimeline::timestamp() const
{
	return QDateTime::currentMSecsSinceEpoch();
}

AbstractTimer *RealTimeline::produceTimer()
{
	return new RealTimer;
}
