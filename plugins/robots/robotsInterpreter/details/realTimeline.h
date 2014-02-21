#pragma once

#include "timelineInterface.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

/// A timeline returning the number of milliseconds since 1970-01-01T00:00:00
class RealTimeline : public TimelineInterface
{
public:
	quint64 timestamp() const override;

	AbstractTimer *produceTimer() override;
};

}
}
}
}
