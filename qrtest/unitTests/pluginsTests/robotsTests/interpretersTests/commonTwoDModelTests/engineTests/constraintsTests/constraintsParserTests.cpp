#include "constraintsParserTests.h"

#include <src/engine/constraints/constraintsChecker.h>
#include <src/engine/constraints/details/event.h>

using namespace qrTest::robotsTests::commonTwoDModelTests;
using namespace twoDModel::constraints::details;

ConstraintsParserTests::ConstraintsParserTests()
	: mParser(mEvents, mVariables, mObjects, mTimeline)
{
}

void ConstraintsParserTests::SetUp()
{
	qDeleteAll(mEvents);
	mEvents.clear();
	mVariables.clear();
	mObjects.clear();
	mTimeline.setTimestamp(0);
}

TEST_F(ConstraintsParserTests, emptyXmlTest)
{
	ASSERT_TRUE(mParser.parse(QString()));
	ASSERT_TRUE(mEvents.isEmpty());
	ASSERT_TRUE(mVariables.isEmpty());
	ASSERT_TRUE(mObjects.isEmpty());
}

TEST_F(ConstraintsParserTests, timeLimitConstraintTest)
{
	const QString xml =
			"<constraints>"\
			"	<timelimit value=\"1000\"/>"\
			"</constraints>";
	ASSERT_TRUE(mParser.parse(xml));
	ASSERT_TRUE(mEvents.count() == 1);
	Event * const event = mEvents.values()[0];
	ASSERT_NE(event, nullptr);

	bool eventFired = false;
	QObject::connect(event, &Event::fired, [&eventFired]() { eventFired = true; });

	mTimeline.setTimestamp(900);
	event->check();
	ASSERT_FALSE(eventFired);

	mTimeline.setTimestamp(1000);
	event->check();
	ASSERT_TRUE(eventFired);
}
