#include "testTimeline.h"

using namespace qrTest::robotsTests::commonTwoDModelTests;

quint64 TestTimeline::timestamp() const
{
	return mTimestamp;
}

utils::AbstractTimer *TestTimeline::produceTimer()
{
	return nullptr;
}

void TestTimeline::setTimestamp(quint64 value)
{
	mTimestamp = value;
}
