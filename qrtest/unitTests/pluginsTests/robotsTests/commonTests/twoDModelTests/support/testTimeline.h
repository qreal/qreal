#pragma once

#include <utils/timelineInterface.h>

namespace qrTest {
namespace robotsTests {
namespace commonTwoDModelTests {

/// A helper class with managed timestamp switching.
class TestTimeline : public utils::TimelineInterface
{
public:
	quint64 timestamp() const override;
	utils::AbstractTimer *produceTimer() override;

	/// Sets current timestamp value; all subsequent timestamp() calls will return
	/// the given value till the other setTimestamp() call.
	void setTimestamp(quint64 value);

private:
	quint64 mTimestamp = 0;
};

}
}
}
