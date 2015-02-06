#include "constraintsParserTests.h"

#include <QtCore/QDebug>

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

TEST_F(ConstraintsParserTests, noTimeLimitErrorTest)
{
	const QString xml = "<constraints/>";
	ASSERT_FALSE(mParser.parse(xml));
	qDebug() << mParser.errors();
	ASSERT_EQ(mParser.errors().count(), 1);
}

TEST_F(ConstraintsParserTests, tooManyTimeLimitErrorTest)
{
	const QString xml =
			"<constraints>"\
			"	<timelimit value=\"1000\"/>"
			"	<timelimit value=\"2000\"/>"
			"</constraints>";
	ASSERT_FALSE(mParser.parse(xml));
	qDebug() << mParser.errors();
	ASSERT_EQ(mParser.errors().count(), 1);
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

TEST_F(ConstraintsParserTests, timerWithoutDropTest)
{
	const QString xml =
			"<constraints>"\
			"	<timelimit value=\"2000\"/>"\
			"	<event id=\"event\" settedUpInitially=\"true\" dropsOnFire=\"false\">"\
			"		<conditions glue=\"and\">"\
			"			<condition>"\
			"				<timer timeout=\"1000\" forceDropOnTimeout=\"false\"/>"\
			"			</condition>"\
			"			<condition>"\
			"				<timer timeout=\"1500\" forceDropOnTimeout=\"false\"/>"\
			"			</condition>"\
			"		</conditions>"\
			"		<trigger>"\
			"			<success/>"
			"		</trigger>"\
			"	</event>"\
			"</constraints>";
	ASSERT_TRUE(mParser.parse(xml));
	ASSERT_TRUE(mEvents.count() == 2);
	Event * const event = mEvents["event"];
	ASSERT_NE(event, nullptr);

	bool eventFired = false;
	QObject::connect(event, &Event::fired, [&eventFired]() { eventFired = true; });

	mTimeline.setTimestamp(1000);
	event->check();
	ASSERT_FALSE(eventFired);

	mTimeline.setTimestamp(1500);
	event->check();
	ASSERT_TRUE(eventFired);

	eventFired = false;
	mTimeline.setTimestamp(1600);
	event->check();
	ASSERT_TRUE(eventFired);
}

TEST_F(ConstraintsParserTests, timerWithDropTest)
{
	const QString xml =
			"<constraints>"\
			"	<timelimit value=\"2000\"/>"\
			"	<event id=\"event\" settedUpInitially=\"true\" dropsOnFire=\"false\">"\
			"		<conditions glue=\"and\">"\
			"			<condition>"\
			"				<timer timeout=\"1000\" forceDropOnTimeout=\"false\"/>"\
			"			</condition>"\
			"			<condition>"\
			"				<timer timeout=\"1500\" forceDropOnTimeout=\"true\"/>"\
			"			</condition>"\
			"		</conditions>"\
			"		<trigger>"\
			"			<success/>"
			"		</trigger>"\
			"	</event>"\
			"</constraints>";
	ASSERT_TRUE(mParser.parse(xml));
	ASSERT_TRUE(mEvents.count() == 2);
	Event * const event = mEvents["event"];
	ASSERT_NE(event, nullptr);

	bool eventFired = false;
	QObject::connect(event, &Event::fired, [&eventFired]() { eventFired = true; });

	mTimeline.setTimestamp(1000);
	event->check();
	ASSERT_FALSE(eventFired);

	mTimeline.setTimestamp(1500);
	event->check();
	ASSERT_TRUE(eventFired);

	eventFired = false;
	mTimeline.setTimestamp(1600);
	event->check();
	ASSERT_FALSE(eventFired);
}

TEST_F(ConstraintsParserTests, noGlueErrorTest)
{
	const QString xml =
			"<constraints>"\
			"	<timelimit value=\"2000\"/>"\
			"	<event id=\"event\">"\
			"		<conditions>"\
			"			<condition>"\
			"				<timer timeout=\"1000\"/>"\
			"			</condition>"\
			"			<condition>"\
			"				<timer timeout=\"1500\"/>"\
			"			</condition>"\
			"		</conditions>"\
			"		<trigger>"\
			"			<success/>"
			"		</trigger>"\
			"	</event>"\
			"</constraints>";
	ASSERT_FALSE(mParser.parse(xml));
	qDebug() << mParser.errors();
	ASSERT_EQ(mParser.errors().count(), 1);
}
