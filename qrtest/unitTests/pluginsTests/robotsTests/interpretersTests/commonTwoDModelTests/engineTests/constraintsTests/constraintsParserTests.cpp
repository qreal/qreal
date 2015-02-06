#include "constraintsParserTests.h"

#include <src/engine/constraints/constraintsChecker.h>
#include <src/engine/constraints/details/event.h>

using namespace qrTest::robotsTests::commonTwoDModelTests;

ConstraintsParserTests::ConstraintsParserTests()
	: mParser(mEvents, mVariables, mObjects, mTimeline)
{
}

void ConstraintsParserTests::SetUp()
{
}

void ConstraintsParserTests::clearEverything()
{
	qDeleteAll(mEvents);
	mEvents.clear();
	mVariables.clear();
	mObjects.clear();
}

TEST_F(ConstraintsParserTests, emptyXmlTest)
{
	clearEverything();
	ASSERT_TRUE(mParser.parse(QString()));
	ASSERT_TRUE(mEvents.isEmpty());
	ASSERT_TRUE(mVariables.isEmpty());
	ASSERT_TRUE(mObjects.isEmpty());
}
