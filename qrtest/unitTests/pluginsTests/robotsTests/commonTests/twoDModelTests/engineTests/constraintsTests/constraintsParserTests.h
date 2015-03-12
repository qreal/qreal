#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <src/engine/constraints/details/constraintsParser.h>
#include "support/testTimeline.h"
#include "support/testObject.h"

namespace qrTest {
namespace robotsTests {
namespace commonTwoDModelTests {

/// Tests for ConstraintsParser.
class ConstraintsParserTests : public testing::Test
{
public:
	ConstraintsParserTests();

protected:
	void SetUp() override;

	twoDModel::constraints::details::Events mEvents;
	twoDModel::constraints::details::Variables mVariables;
	twoDModel::constraints::details::Objects mObjects;
	TestObject mTestObject;
	TestTimeline mTimeline;
	twoDModel::constraints::details::StatusReporter mStatus;
	twoDModel::constraints::details::ConstraintsParser mParser;
};

}
}
}
