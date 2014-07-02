#pragma once

#include "timelineInterface.h"
#include "utilsDeclSpec.h"

namespace utils {

/// A timeline returning the number of milliseconds since 1970-01-01T00:00:00
class ROBOTS_UTILS_EXPORT RealTimeline : public TimelineInterface
{
public:
	quint64 timestamp() const override;
	AbstractTimer *produceTimer() override;
};

}
