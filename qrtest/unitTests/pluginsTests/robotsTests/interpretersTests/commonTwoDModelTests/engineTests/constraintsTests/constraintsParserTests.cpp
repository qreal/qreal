#include "constraintsParserTests.h"

#include <QtCore/QDebug>

#include <src/engine/constraints/constraintsChecker.h>
#include <src/engine/constraints/details/event.h>

using namespace qrTest::robotsTests::commonTwoDModelTests;
using namespace twoDModel::constraints::details;

ConstraintsParserTests::ConstraintsParserTests()
	: mParser(mEvents, mVariables, mObjects, mTimeline, mStatus)
{
}

void ConstraintsParserTests::SetUp()
{
	qDeleteAll(mEvents);
	mEvents.clear();
	mVariables.clear();
	mObjects.clear();
	mTimeline.setTimestamp(0);

	mObjects["testObject"] = &mTestObject;
	mObjects["nullObject"] = nullptr;
}

TEST_F(ConstraintsParserTests, emptyXmlTest)
{
	ASSERT_TRUE(mParser.parse(QString()));
	ASSERT_TRUE(mEvents.isEmpty());
	ASSERT_TRUE(mVariables.isEmpty());
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
	ASSERT_EQ(mEvents.count(), 1);
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
	ASSERT_EQ(mEvents.count(), 2);
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
	ASSERT_EQ(mEvents.count(), 2);
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

TEST_F(ConstraintsParserTests, comparisonTest)
{
	auto testSign = [this](const QString &sign, const QString &type) {
		mEvents.clear();
		const QString xml = QString(
				"<constraints>"\
				"	<timelimit value=\"2000\"/>"\
				"	<event id=\"event\" settedUpInitially=\"true\">"\
				"		<condition>"\
				"			<%1>"\
				"				<objectState objectId=\"testObject\" property=\"%2Property\"/>"\
				"				<%2 value=\"100\"/>"\
				"			</%1>"\
				"		</condition>"\
				"		<trigger>"\
				"			<success/>"
				"		</trigger>"\
				"	</event>"\
				"</constraints>").arg(sign, type);
		ASSERT_TRUE(mParser.parse(xml));
		ASSERT_EQ(mEvents.count(), 2);
		Event * const event = mEvents["event"];
		ASSERT_NE(event, nullptr);

		bool eventFired = false;
		QObject::connect(event, &Event::fired, [&eventFired]() { eventFired = true; });

		event->check();
		ASSERT_TRUE(eventFired);
	};

	mTestObject.setIntProperty(100);
	testSign("equals", "int");
	mTestObject.setIntProperty(101);
	testSign("notEqual", "int");

	mTestObject.setIntProperty(1000);
	testSign("greater", "int");
	testSign("notLess", "int");

	mTestObject.setDoubleProperty(99.1);
	testSign("less", "double");
	mTestObject.setDoubleProperty(100.0);
	testSign("notGreater", "double");

	mTestObject.setStringProperty("100");
	testSign("equals", "string");
	mTestObject.setStringProperty("string value");
	testSign("notEquals", "string");
	mTestObject.setStringProperty("10");
	testSign("less", "string");
	mTestObject.setStringProperty("99");
	testSign("greater", "string");
}

TEST_F(ConstraintsParserTests, constraintTagAndTypeOfTagTest)
{
	auto testCase = [this](bool checkOnce, const QString &objectId, const QString &type) {
		mEvents.clear();
		const QString xml = QString(
				"<constraints>"\
				"	<timelimit value=\"2000\"/>"\
				"	<constraint id=\"constraint\" checkOnce=\"%1\" failMessage=\"fail!\">"\
				"		<equals>"
				"			<typeof objectId=\"%2\"/>"
				"			<string value=\"%3\"/>"
				"		</equals>"
				"	</constraint>"\
				"</constraints>").arg(checkOnce ? "true" : "false", objectId, type);
		ASSERT_TRUE(mParser.parse(xml));
		ASSERT_EQ(mEvents.count(), 2);
		Event * const event = mEvents["constraint"];
		ASSERT_NE(event, nullptr);

		bool eventFired = false;
		QObject::connect(event, &Event::fired, [&eventFired]() { eventFired = true; });

		event->check();
		ASSERT_FALSE(eventFired);
		ASSERT_NE(checkOnce, event->isAlive());
	};

	testCase(true, "testObject", "qrTest::robotsTests::commonTwoDModelTests::TestObject");
	testCase(false, "nullObject", "undefined");
}

TEST_F(ConstraintsParserTests, forgottenFailMessageErrorTest)
{
	const QString xml =
			"<constraints>"\
			"	<timelimit value=\"2000\"/>"\
			"	<constraint id=\"constraint\" checkOnce=\"false\">"\
			"		<equals>"
			"			<typeof objectId=\"nullObject\"/>"
			"			<string value=\"undefined\"/>"
			"		</equals>"
			"	</constraint>"\
			"</constraints>";
	ASSERT_FALSE(mParser.parse(xml));
	qDebug() << mParser.errors();
	ASSERT_EQ(mParser.errors().count(), 1);
}

TEST_F(ConstraintsParserTests, setVariableTest)
{
	auto testCase = [this](const QString &value) {
		mEvents.clear();
		mVariables.clear();
		mTimeline.setTimestamp(0);
		const QString xml = QString(
				"<constraints>"\
				"	<timelimit value=\"2000\"/>"\
				"	<event id=\"event\" settedUpInitially=\"true\">"\
				"		<condition>"\
				"			<timer timeout=\"1\"/>"\
				"		</condition>"\
				"		<trigger>"\
				"			<setVariable name=\"x\" value=\"%1\"/>"
				"		</trigger>"\
				"	</event>"\
				"</constraints>").arg(value);
		ASSERT_TRUE(mParser.parse(xml));
		ASSERT_EQ(mEvents.count(), 2);
		Event * const event = mEvents["event"];
		ASSERT_NE(event, nullptr);

		bool eventFired = false;
		QObject::connect(event, &Event::fired, [&eventFired]() { eventFired = true; });

		mTimeline.setTimestamp(1);
		event->check();
		ASSERT_TRUE(eventFired);
	};

	testCase("100500");
	ASSERT_TRUE(mVariables.contains("x"));
	ASSERT_EQ(mVariables["x"].type(), QVariant::Int);
	ASSERT_EQ(mVariables["x"].toInt(), 100500);

	testCase("100500.2");
	ASSERT_TRUE(mVariables.contains("x"));
	ASSERT_EQ(mVariables["x"].type(), QVariant::Double);
	ASSERT_EQ(mVariables["x"].toDouble(), 100500.2);

	testCase("abc");
	ASSERT_TRUE(mVariables.contains("x"));
	ASSERT_EQ(mVariables["x"].type(), QVariant::String);
	ASSERT_EQ(mVariables["x"].toString(), "abc");
}

TEST_F(ConstraintsParserTests, addToVariableTest)
{
	auto testCase = [this](const QString &value) {
		mEvents.clear();
		mVariables.clear();
		mTimeline.setTimestamp(0);
		mVariables["x"] = 100500;
		const QString xml = QString(
				"<constraints>"\
				"	<timelimit value=\"2000\"/>"\
				"	<event id=\"event\" settedUpInitially=\"true\">"\
				"		<condition>"\
				"			<timer timeout=\"1\"/>"\
				"		</condition>"\
				"		<trigger>"\
				"			<addToVariable name=\"x\" value=\"%1\"/>"
				"		</trigger>"\
				"	</event>"\
				"</constraints>").arg(value);
		ASSERT_TRUE(mParser.parse(xml));
		ASSERT_EQ(mEvents.count(), 2);
		Event * const event = mEvents["event"];
		ASSERT_NE(event, nullptr);

		bool eventFired = false;
		QObject::connect(event, &Event::fired, [&eventFired]() { eventFired = true; });

		mTimeline.setTimestamp(1);
		event->check();
		ASSERT_TRUE(eventFired);
	};

	testCase("100500");
	ASSERT_TRUE(mVariables.contains("x"));
	ASSERT_EQ(mVariables["x"].type(), QVariant::Int);
	ASSERT_EQ(mVariables["x"].toInt(), 100500 + 100500);

	testCase("100500.2");
	ASSERT_TRUE(mVariables.contains("x"));
	ASSERT_EQ(mVariables["x"].type(), QVariant::Double);
	ASSERT_EQ(mVariables["x"].toDouble(), 100500.2 + 100500);

	testCase("abc");
	ASSERT_TRUE(mVariables.contains("x"));
	ASSERT_EQ(mVariables["x"].type(), QVariant::String);
	ASSERT_EQ(mVariables["x"].toString(), "100500abc");
}

TEST_F(ConstraintsParserTests, variableValueTest)
{
	auto testCase = [this](const QString &value, const QString &type) {
		mEvents.clear();
		const QString xml = QString(
				"<constraints>"\
				"	<timelimit value=\"2000\"/>"\
				"	<event id=\"event\" settedUpInitially=\"true\">"\
				"		<condition>"\
				"			<equals>"
				"				<variableValue name=\"x\"/>"
				"				<%1 value=\"%2\"/>"
				"			</equals>"
				"		</condition>"\
				"		<trigger>"\
				"			<success/>"
				"		</trigger>"\
				"	</event>"\
				"</constraints>").arg(type, value);
		ASSERT_TRUE(mParser.parse(xml));
		ASSERT_EQ(mEvents.count(), 2);
		Event * const event = mEvents["event"];
		ASSERT_NE(event, nullptr);

		bool eventFired = false;
		QObject::connect(event, &Event::fired, [&eventFired]() { eventFired = true; });

		event->check();
		ASSERT_TRUE(eventFired);
	};

	mVariables["x"] = -1;
	testCase("-1", "int");

	mVariables["x"] = -1.2;
	testCase("-1.2", "double");

	mVariables["x"] = "test";
	testCase("test", "string");
}

TEST_F(ConstraintsParserTests, communicationTest)
{
	// This program will iteratively add 2 to 'counter', then subtract 1, then again add 2 and so on...
	const QString xml =
			"<constraints>"\
			"	<timelimit value=\"2000\"/>"\
			"	<event id=\"Set Initial Value\" settedUpInitially=\"true\">"\
			"		<condition>"\
			"			<timer timeout=\"0\"/>"
			"		</condition>"\
			"		<triggers>"
			"			<trigger>"\
			"				<setVariable name=\"counter\" value=\"0\"/>"
			"			</trigger>"\
			"			<trigger>"\
			"				<setUp id=\"Increment 2\" value=\"0\"/>"
			"			</trigger>"\
			"		</triggers>"
			"	</event>"\
			"	<event id=\"Check Done\" settedUpInitially=\"true\">"\
			"		<condition>"\
			"			<notLess>"
			"				<variableValue name=\"counter\"/>"
			"				<int value=\"10\"/>"
			"			</notLess>"
			"		</condition>"\
			"		<trigger>"\
			"			<success/>"
			"		</trigger>"\
			"	</event>"\
			"	<event id=\"Increment 2\" settedUpInitially=\"true\" dropsOnFire=\"false\">"\
			"		<condition>"\
			"			<timer timeout=\"1\"/>"
			"		</condition>"\
			"		<triggers>"\
			"			<addToVariable name=\"counter\" value=\"2\" />"
			"			<setUp id=\"Decrement 1\" />"
			"			<drop id=\"Increment 2\" />"
			"		</triggers>"\
			"	</event>"\
			"	<event id=\"Decrement 1\" settedUpInitially=\"true\">"\
			"		<condition>"\
			"			<timer timeout=\"1\"/>"
			"		</condition>"\
			"		<triggers>"\
			"			<addToVariable name=\"counter\" value=\"-1\" />"
			"			<setUp id=\"Increment 2\" />"
			"		</triggers>"\
			"	</event>"\
			"</constraints>";
	ASSERT_TRUE(mParser.parse(xml));
	ASSERT_EQ(mEvents.count(), 5);

	QMap<QString, int> fireCounters;

	for (const QString &eventId : mEvents.keys()) {
		Event const *event = mEvents[eventId];
		ASSERT_NE(event, nullptr);
		fireCounters[eventId] = 0;
		QObject::connect(event, &Event::fired, [&fireCounters, eventId]() { ++fireCounters[eventId]; });
	}

	const int timeout = 2000;
	const QString &doneEventId = "Check Done";
	int time = 0;
	while (time <= timeout && !fireCounters[doneEventId]) {
		mTimeline.setTimestamp(time);
		for (Event * const event : mEvents.values()) {
			event->check();
		}

		++time;
	}

	ASSERT_LT(time, timeout);
	ASSERT_EQ(fireCounters[doneEventId], 1);
	ASSERT_EQ(fireCounters["Set Initial Value"], 1);
	ASSERT_EQ(fireCounters["Increment 2"], 10);
	ASSERT_EQ(fireCounters["Decrement 1"], 11);
	ASSERT_EQ(fireCounters["Set Initial Value"], 1);
}

/// @todo: Add inside tag testcases
