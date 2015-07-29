/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "constraintsParserTests.h"

#include <QtCore/QDebug>

#include <utils/objectsSet.h>
#include <utils/canvas/pointObject.h>
#include <utils/canvas/rectangleObject.h>
#include <utils/canvas/lineObject.h>
#include <utils/canvas/ellipseObject.h>
#include <src/engine/constraints/constraintsChecker.h>
#include <src/engine/constraints/details/event.h>

using namespace qrTest::robotsTests::commonTwoDModelTests;
using namespace twoDModel::constraints::details;
using namespace utils;

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
			"<constraints>"
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
			"<constraints>"
			"	<timelimit value=\"1000\"/>"
			"</constraints>";
	ASSERT_TRUE(mParser.parse(xml));
	ASSERT_EQ(mEvents.count(), 1);
	Event * const event = mEvents.values()[0];
	ASSERT_NE(event, nullptr);
	event->setUp();

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
			"<constraints>"
			"	<timelimit value=\"2000\"/>"
			"	<event id=\"event\" settedUpInitially=\"true\" dropsOnFire=\"false\">"
			"		<conditions glue=\"and\">"
			"			<condition>"
			"				<timer timeout=\"1000\" forceDropOnTimeout=\"false\"/>"
			"			</condition>"
			"			<condition>"
			"				<timer timeout=\"1500\" forceDropOnTimeout=\"false\"/>"
			"			</condition>"
			"		</conditions>"
			"		<trigger>"
			"			<success/>"
			"		</trigger>"
			"	</event>"
			"</constraints>";
	ASSERT_TRUE(mParser.parse(xml));
	ASSERT_EQ(mEvents.count(), 2);
	Event * const event = mEvents["event"];
	ASSERT_NE(event, nullptr);
	event->setUp();

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
			"<constraints>"
			"	<timelimit value=\"2000\"/>"
			"	<event id=\"event\" settedUpInitially=\"true\" dropsOnFire=\"false\">"
			"		<conditions glue=\"and\">"
			"			<condition>"
			"				<timer timeout=\"1000\" forceDropOnTimeout=\"false\"/>"
			"			</condition>"
			"			<condition>"
			"				<timer timeout=\"1500\" forceDropOnTimeout=\"true\"/>"
			"			</condition>"
			"		</conditions>"
			"		<trigger>"
			"			<success/>"
			"		</trigger>"
			"	</event>"
			"</constraints>";
	ASSERT_TRUE(mParser.parse(xml));
	ASSERT_EQ(mEvents.count(), 2);
	Event * const event = mEvents["event"];
	ASSERT_NE(event, nullptr);
	event->setUp();

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
			"<constraints>"
			"	<timelimit value=\"2000\"/>"
			"	<event id=\"event\">"
			"		<conditions>"
			"			<condition>"
			"				<timer timeout=\"1000\"/>"
			"			</condition>"
			"			<condition>"
			"				<timer timeout=\"1500\"/>"
			"			</condition>"
			"		</conditions>"
			"		<trigger>"
			"			<success/>"
			"		</trigger>"
			"	</event>"
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
				"<constraints>"
				"	<timelimit value=\"2000\"/>"
				"	<event id=\"event\" settedUpInitially=\"true\">"
				"		<condition>"
				"			<%1>"
				"				<objectState object=\"testObject.%2Property\"/>"
				"				<%2 value=\"100\"/>"
				"			</%1>"
				"		</condition>"
				"		<trigger>"
				"			<success/>"
				"		</trigger>"
				"	</event>"
				"</constraints>").arg(sign, type);
		ASSERT_TRUE(mParser.parse(xml));
		ASSERT_EQ(mEvents.count(), 2);
		Event * const event = mEvents["event"];
		ASSERT_NE(event, nullptr);
		event->setUp();

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
				"<constraints>"
				"	<timelimit value=\"2000\"/>"
				"	<constraint id=\"constraint\" checkOnce=\"%1\" failMessage=\"fail!\">"
				"		<equals>"
				"			<typeof objectId=\"%2\"/>"
				"			<string value=\"%3\"/>"
				"		</equals>"
				"	</constraint>"
				"</constraints>").arg(checkOnce ? "true" : "false", objectId, type);
		ASSERT_TRUE(mParser.parse(xml));
		ASSERT_EQ(mEvents.count(), 2);
		Event * const event = mEvents["constraint"];
		ASSERT_NE(event, nullptr);
		event->setUp();

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
			"<constraints>"
			"	<timelimit value=\"2000\"/>"
			"	<constraint id=\"constraint\" checkOnce=\"false\">"
			"		<equals>"
			"			<typeof objectId=\"nullObject\"/>"
			"			<string value=\"undefined\"/>"
			"		</equals>"
			"	</constraint>"
			"</constraints>";
	ASSERT_FALSE(mParser.parse(xml));
	qDebug() << mParser.errors();
	ASSERT_EQ(mParser.errors().count(), 1);
}

TEST_F(ConstraintsParserTests, arithmeticTest)
{
	const QString xml =
			"<constraints>"
			"	<timelimit value=\"2000\"/>"
			"	<event id=\"event1\">"
			"		<condition>"
			"			<timer timeout=\"100\" forceDropOnTimeout=\"true\"/>"
			"		</condition>"
			"		<triggers>"
			"			<setter name=\"x\">"
			"				<int value=\"10\"/>"
			"			</setter>"
			"			<setter name=\"d\">"
			"				<double value=\"10.2\"/>"
			"			</setter>"
			"			<setter name=\"s\">"
			"				<string value=\"abc\"/>"
			"			</setter>"
			"			<setter name=\"b\">"
			"				<bool value=\"true\"/>"
			"			</setter>"
			"		</triggers>"
			"	</event>"
			"	<event id=\"event2\">"
			"		<condition>"
			"			<timer timeout=\"200\" forceDropOnTimeout=\"true\"/>"
			"		</condition>"
			"		<trigger>"
			"			<setter name=\"y\">"
			"				<sum>"
			"					<min>"
			"						<abs>"
			"							<int value=\"-2\"/>"
			"						</abs>"
			"						<int value=\"10\"/>"
			"					</min>"
			"					<variableValue name=\"x\"/>"
			"				</sum>"
			"			</setter>"
			"		</trigger>"
			"	</event>"
			"	<event id=\"event3\">"
			"		<condition>"
			"			<timer timeout=\"300\" forceDropOnTimeout=\"true\"/>"
			"		</condition>"
			"		<trigger>"
			"			<setter name=\"x\">"
			"				<minus>"
			"					<max>"
			"						<int value=\"-4\"/>"
			"						<difference>"
			"							<variableValue name=\"x\"/>"
			"							<variableValue name=\"y\"/>"
			"						</difference>"
			"					</max>"
			"				</minus>"
			"			</setter>"
			"		</trigger>"
			"	</event>"
			"</constraints>";

	ASSERT_TRUE(mParser.parse(xml));
	ASSERT_EQ(mEvents.count(), 4);

	Event * const event1 = mEvents["event1"];
	ASSERT_NE(event1, nullptr);
	event1->setUp();
	Event * const event2 = mEvents["event2"];
	ASSERT_NE(event2, nullptr);
	event2->setUp();
	Event * const event3 = mEvents["event3"];
	ASSERT_NE(event3, nullptr);
	event3->setUp();

	bool event1Fired = false;
	bool event2Fired = false;
	bool event3Fired = false;
	QObject::connect(event1, &Event::fired, [&event1Fired]() { event1Fired = true; });
	QObject::connect(event2, &Event::fired, [&event2Fired]() { event2Fired = true; });
	QObject::connect(event3, &Event::fired, [&event3Fired]() { event3Fired = true; });

	mTimeline.setTimestamp(100);
	event1->check();
	ASSERT_TRUE(event1Fired);
	ASSERT_TRUE(mVariables.contains("x"));
	ASSERT_FALSE(mVariables.contains("y"));
	ASSERT_EQ(mVariables["x"].type(), QVariant::Int);
	ASSERT_EQ(mVariables["x"].toInt(), 10);
	ASSERT_TRUE(mVariables.contains("d"));
	ASSERT_EQ(mVariables["d"].type(), QVariant::Double);
	ASSERT_EQ(mVariables["d"].toDouble(), 10.2);
	ASSERT_TRUE(mVariables.contains("s"));
	ASSERT_EQ(mVariables["s"].type(), QVariant::String);
	ASSERT_EQ(mVariables["s"].toString(), "abc");
	ASSERT_TRUE(mVariables.contains("b"));
	ASSERT_EQ(mVariables["b"].type(), QVariant::Bool);
	ASSERT_EQ(mVariables["b"].toBool(), true);

	mTimeline.setTimestamp(200);
	event2->check();
	ASSERT_TRUE(event2Fired);
	ASSERT_TRUE(mVariables.contains("x"));
	ASSERT_TRUE(mVariables.contains("y"));
	ASSERT_EQ(mVariables["x"].type(), QVariant::Int);
	ASSERT_EQ(mVariables["x"].toInt(), 10);
	ASSERT_EQ(mVariables["y"].type(), QVariant::Int);
	ASSERT_EQ(mVariables["y"].toInt(), 12);

	mTimeline.setTimestamp(300);
	event3->check();
	ASSERT_TRUE(event3Fired);
	ASSERT_TRUE(mVariables.contains("x"));
	ASSERT_TRUE(mVariables.contains("y"));
	ASSERT_EQ(mVariables["x"].type(), QVariant::Int);
	ASSERT_EQ(mVariables["x"].toInt(), 2);
	ASSERT_EQ(mVariables["y"].type(), QVariant::Int);
	ASSERT_EQ(mVariables["y"].toInt(), 12);
}

TEST_F(ConstraintsParserTests, variableValueTest)
{
	auto testCase = [this](const QString &variable, const QString &value, const QString &type) {
		mEvents.clear();
		const QString xml = QString(
				"<constraints>"
				"	<timelimit value=\"2000\"/>"
				"	<event id=\"event\" settedUpInitially=\"true\">"
				"		<condition>"
				"			<equals>"
				"				<variableValue name=\"%1\"/>"
				"				<%2 value=\"%3\"/>"
				"			</equals>"
				"		</condition>"
				"		<trigger>"
				"			<success/>"
				"		</trigger>"
				"	</event>"
				"</constraints>").arg(variable, type, value);
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
	testCase("x", "-1", "int");

	mVariables["x"] = -1.2;
	testCase("x", "-1.2", "double");

	mVariables["x"] = "test";
	testCase("x", "test", "string");

	mVariables["x"] = QRect(1, 2, 3, 4);
	testCase("x.x", "1", "int");
	testCase("x.y", "2", "int");
	testCase("x.width", "3", "int");
	testCase("x.height", "4", "int");
}

TEST_F(ConstraintsParserTests, objectStateTest)
{
	auto testCase = [this](const QString &object, const QString &value, const QString &type) {
		mEvents.clear();
		const QString xml = QString(
				"<constraints>"
				"	<timelimit value=\"2000\"/>"
				"	<event id=\"event\" settedUpInitially=\"true\">"
				"		<condition>"
				"			<equals>"
				"				<objectState object=\"%1\"/>"
				"				<%2 value=\"%3\"/>"
				"			</equals>"
				"		</condition>"
				"		<trigger>"
				"			<success/>"
				"		</trigger>"
				"	</event>"
				"</constraints>").arg(object, type, value);
		ASSERT_TRUE(mParser.parse(xml));
		ASSERT_EQ(mEvents.count(), 2);
		Event * const event = mEvents["event"];
		ASSERT_NE(event, nullptr);

		bool eventFired = false;
		QObject::connect(event, &Event::fired, [&eventFired]() { eventFired = true; });

		event->check();
		ASSERT_TRUE(eventFired);
	};

	TestObjectB *objB = new TestObjectB;
	objB->otherObject()->setIntProperty(100500);
	objB->otherObject()->setStringProperty("abc");
	objB->otherObject()->setRectProperty(QRect(1, 2, 3, 4));
	objB->otherObject()->setPointProperty(QPoint(10, 20));
	mObjects["object"] = objB;

	testCase("object.otherObject.intProperty", "100500", "int");
	testCase("object.otherObject.stringProperty", "abc", "string");
	testCase("object.otherObject.rectProperty.x", "1", "int");
	testCase("object.otherObject.rectProperty.y", "2", "int");
	testCase("object.otherObject.rectProperty.width", "3", "int");
	testCase("object.otherObject.rectProperty.height", "4", "int");
	testCase("object.otherObject.pointProperty.x", "10", "int");
	testCase("object.otherObject.pointProperty.y", "20", "int");
}

TEST_F(ConstraintsParserTests, setObjectStateTest)
{
	auto testCase = [this](const QString &object, const QString &property, const QString &valueXml) {
		mEvents.clear();
		const QString xml = QString(
				"<constraints>"
				"	<timelimit value=\"2000\"/>"
				"	<event id=\"event\" settedUpInitially=\"true\">"
				"		<condition>"
				"			<greater>"
				"				<int value=\"2\"/>"
				"				<int value=\"1\"/>"
				"			</greater>"
				"		</condition>"
				"		<trigger>"
				"			<setState object=\"%1\" property=\"%2\">"
				"				%3"
				"			</setState>"
				"		</trigger>"
				"	</event>"
				"</constraints>").arg(object, property, valueXml);
		ASSERT_TRUE(mParser.parse(xml));
		ASSERT_EQ(mEvents.count(), 2);
		Event * const event = mEvents["event"];
		ASSERT_NE(event, nullptr);

		bool eventFired = false;
		QObject::connect(event, &Event::fired, [&eventFired]() { eventFired = true; });

		event->check();
		ASSERT_TRUE(eventFired);
	};

	TestObjectB *objB = new TestObjectB;
	objB->otherObject()->setIntProperty(100500);
	objB->otherObject()->setStringProperty("abc");
	mObjects["object"] = objB;

	const QString increment11Xml =
			"<sum>"
			"	<objectState object=\"object.otherObject.intProperty\"/>"
			"	<int value=\"11\"/>"
			"</sum>";
	testCase("object.otherObject", "intProperty", increment11Xml);
	ASSERT_EQ(objB->otherObject()->intProperty(), 100511);
	testCase("object.otherObject", "stringProperty", "<string value=\"abc123\"/>");
	ASSERT_EQ(objB->otherObject()->stringProperty(), "abc123");
}

TEST_F(ConstraintsParserTests, objectsSetTest)
{
	auto testCase = [this](const QString &object, int size, int first, int last) {
		mEvents.clear();
		const QString xml = QString(
				"<constraints>"
				"	<timelimit value=\"2000\"/>"
				"	<event id=\"event\" settedUpInitially=\"true\">"
				"		<conditions glue=\"and\">"
				"			<equals>"
				"				<objectState object=\"%1.size\"/>"
				"				<int value=\"%2\"/>"
				"			</equals>"
				"			<equals>"
				"				<objectState object=\"%1.first\"/>"
				"				<int value=\"%3\"/>"
				"			</equals>"
				"			<equals>"
				"				<objectState object=\"%1.last\"/>"
				"				<int value=\"%4\"/>"
				"			</equals>"
				"			<equals>"
				"				<objectState object=\"%1.isEmpty\"/>"
				"				<bool value=\"false\"/>"
				"			</equals>"
				"		</conditions>"
				"		<trigger>"
				"			<success/>"
				"		</trigger>"
				"	</event>"
				"</constraints>").arg(object, QString::number(size), QString::number(first), QString::number(last));
		ASSERT_TRUE(mParser.parse(xml));
		ASSERT_EQ(mEvents.count(), 2);
		Event * const event = mEvents["event"];
		ASSERT_NE(event, nullptr);

		bool eventFired = false;
		QObject::connect(event, &Event::fired, [&eventFired]() { eventFired = true; });

		event->check();
		ASSERT_TRUE(eventFired);
	};

	VariantSet * const set = new VariantSet;
	set->add(10);
	set->add(20);
	set->add(30);
	mObjects["set"] = set;
	testCase("set", set->size(), set->first().toInt(), set->last().toInt());
	delete set;

	TestObjectA * const object = new TestObjectA();
	object->setIntListProperty({10, 9, 8, 7, 6, 5, 4, 3, 2, 1});
	mObjects["object"] = object;
	testCase("object.intListProperty", object->intListProperty().size()
			, object->intListProperty().first(), object->intListProperty().last());
}

TEST_F(ConstraintsParserTests, boundingRectTest)
{
	auto testCase = [this](const QString &prefix, int x, int y, int width, int height) {
		mEvents.clear();
		const QString xml = QString(
				"<constraints>"
				"	<timelimit value=\"2000\"/>"
				"	<event id=\"event\" settedUpInitially=\"true\">"
				"		<condition>"
				"			<using>"
				"				<setter name=\"boundingRect\">"
				"					<boundingRect>"
				"						<%1=\"items\"/>"
				"					</boundingRect>"
				"				</setter>"
				"				<return>"
				"					<conditions glue=\"and\">"
				"						<equals>"
				"							<variableValue name=\"boundingRect.x\"/>"
				"							<int value=\"%2\"/>"
				"						</equals>"
				"						<equals>"
				"							<variableValue name=\"boundingRect.y\"/>"
				"							<int value=\"%3\"/>"
				"						</equals>"
				"						<equals>"
				"							<variableValue name=\"boundingRect.width\"/>"
				"							<int value=\"%4\"/>"
				"						</equals>"
				"						<equals>"
				"							<variableValue name=\"boundingRect.height\"/>"
				"							<int value=\"%5\"/>"
				"						</equals>"
				"					</conditions>"
				"				</return>"
				"			</using>"
				"		</condition>"
				"		<trigger>"
				"			<success/>"
				"		</trigger>"
				"	</event>"
				"</constraints>").arg(prefix
						, QString::number(x)
						, QString::number(y)
						, QString::number(width)
						, QString::number(height));
		ASSERT_TRUE(mParser.parse(xml));
		ASSERT_EQ(mEvents.count(), 2);
		Event * const event = mEvents["event"];
		ASSERT_NE(event, nullptr);

		bool eventFired = false;
		QObject::connect(event, &Event::fired, [&eventFired]() { eventFired = true; });

		event->check();
		ASSERT_TRUE(eventFired);
	};

	const QList<CanvasObject *> shapes = {
			new EllipseObject(QPoint(0, 0), 50, 50)
			, new RectangleObject(QRect(0, 0, 200, 200))
			, new LineObject(QPoint(-120, -130), QPoint(-80, -70))
			, new PointObject(201, 201) };
	const QRect correctRect(QPoint(-120, -130), QPoint(201, 201));

	ObjectsSet<CanvasObject *> set(shapes);
	mObjects["items"] = &set;
	testCase("objectState object", correctRect.left(), correctRect.top(), correctRect.width(), correctRect.height());

	VariantSet emptySet;
	mObjects["items"] = &emptySet;
	testCase("objectState object", 0, 0, 0, 0);

	mVariables["items"] = QVariant::fromValue<QList<CanvasObject *>>(shapes);
	testCase("variableValue name", correctRect.left(), correctRect.top(), correctRect.width(), correctRect.height());
}

TEST_F(ConstraintsParserTests, distanceTest)
{
	auto testCase = [this](const QString &value) {
		mEvents.clear();
		const QString xml = QString(
				"<constraints>"
				"	<timelimit value=\"2000\"/>"
				"	<event id=\"event\" settedUpInitially=\"true\">"
				"		<condition>"
				"			<equals>"
				"				<distance>"
				"					<variableValue name=\"point1\"/>"
				"					<variableValue name=\"point2\"/>"
				"				</distance>"
				"				<double value=\"%1\"/>"
				"			</equals>"
				"		</condition>"
				"		<trigger>"
				"			<success/>"
				"		</trigger>"
				"	</event>"
				"</constraints>").arg(value);
		ASSERT_TRUE(mParser.parse(xml));
		ASSERT_EQ(mEvents.count(), 2);
		Event * const event = mEvents["event"];
		ASSERT_NE(event, nullptr);

		bool eventFired = false;
		QObject::connect(event, &Event::fired, [&eventFired]() { eventFired = true; });

		event->check();
		ASSERT_TRUE(eventFired);
	};

	mVariables["point1"] = QPoint(0, 0);
	mVariables["point2"] = QPoint(400, 300);
	testCase("500");

	mVariables["point1"] = QPoint(0, 0);
	mVariables["point2"] = QPoint(0, 0);
	testCase("0");

	mVariables["point1"] = 3;
	mVariables["point2"] = 4;
	testCase("0");
}

TEST_F(ConstraintsParserTests, usingTest)
{
	const QString xml =
			"<constraints>"
			"	<timelimit value=\"2000\"/>"
			"	<event id=\"event1\">"
			"		<condition>"
			"			<using>"
			"				<setter name=\"x\">"
			"					<int value=\"100500\"/>"
			"				</setter>"
			"				<setter name=\"y\">"
			"					<int value=\"1\"/>"
			"				</setter>"
			"				<return>"
			"					<greater>"
			"						<variableValue name=\"y\"/>"
			"						<int value=\"0\"/>"
			"					</greater>"
			"				</return>"
			"			</using>"
			"		</condition>"
			"		<trigger>"
			"			<setUp id=\"event2\"/>"
			"		</trigger>"
			"	</event>"
			"	<event id=\"event2\">"
			"		<conditions glue=\"and\">"
			"			<timer timeout=\"500\"/>"
			"			<equals>"
			"				<variableValue name=\"x\"/>"
			"				<int value=\"100500\"/>"
			"			</equals>"
			"		</conditions>"
			"		<trigger>"
			"			<success/>"
			"		</trigger>"
			"	</event>"
			"</constraints>";
	ASSERT_TRUE(mParser.parse(xml));
	ASSERT_EQ(mEvents.count(), 3);

	Event * const event1 = mEvents["event1"];
	ASSERT_NE(event1, nullptr);
	event1->setUp();
	Event * const event2 = mEvents["event2"];
	ASSERT_NE(event2, nullptr);
	// event2 is not setted up initially

	bool event1Fired = false;
	bool event2Fired = false;
	QObject::connect(event1, &Event::fired, [&event1Fired]() { event1Fired = true; });
	QObject::connect(event2, &Event::fired, [&event2Fired]() { event2Fired = true; });

	mTimeline.setTimestamp(100);
	event1->check();
	ASSERT_TRUE(event1Fired);
	ASSERT_TRUE(event2->isAlive());
	ASSERT_FALSE(event2Fired);
	ASSERT_TRUE(mVariables.contains("x"));
	ASSERT_TRUE(mVariables.contains("y"));
	ASSERT_EQ(mVariables["x"].type(), QVariant::Int);
	ASSERT_EQ(mVariables["y"].type(), QVariant::Int);
	ASSERT_EQ(mVariables["x"].toInt(), 100500);
	ASSERT_EQ(mVariables["y"].toInt(), 1);

	// event2 was last time setted up at timestamp 100, so it will trigger at timestamp 600.
	mTimeline.setTimestamp(600);
	event2->check();
	ASSERT_TRUE(event2Fired);
}

TEST_F(ConstraintsParserTests, forgottenReturnInUsingTest)
{
	const QString xml =
			"<constraints>"
			"	<timelimit value=\"2000\"/>"
			"	<event>"
			"		<condition>"
			"			<using>"
			"				<setter name=\"x\">"
			"					<int value=\"100500\"/>"
			"				</setter>"
			"				<setter name=\"y\">"
			"					<int value=\"1\"/>"
			"				</setter>"
			"				<greater>"
			"					<variableValue name=\"y\"/>"
			"					<int value=\"0\"/>"
			"				</greater>"
			"			</using>"
			"		</condition>"
			"		<trigger>"
			"			<success/>"
			"		</trigger>"
			"	</event>"
			"</constraints>";
	ASSERT_FALSE(mParser.parse(xml));
	qDebug() << mParser.errors();
	// 2 errors: triggers has no 'greater' variant + no 'return' tag error
	ASSERT_EQ(mParser.errors().count(), 2);
}

TEST_F(ConstraintsParserTests, initializationTagTest)
{
	const QString xml =
			"<constraints>"
			"	<timelimit value=\"2000\"/>"
			"	<init>"
			"		<setter name=\"x\"><int value=\"123\"/></setter>"
			"	</init>"
			"	<event id=\"event\" settedUpInitially=\"true\">"
			"		<conditions glue=\"and\">"
			"			<timer timeout=\"1000\"/>"
			"			<equals>"
			"				<variableValue name=\"x\"/>"
			"				<int value=\"123\"/>"
			"			</equals>"
			"		</conditions>"
			"		<trigger>"
			"			<success/>"
			"		</trigger>"
			"	</event>"
			"</constraints>";
	ASSERT_TRUE(mParser.parse(xml));
	ASSERT_EQ(mEvents.count(), 3);

	for (Event * const event : mEvents.values()) {
		ASSERT_NE(event, nullptr);
		event->setUp();
		event->check();
	}

	Event * const event = mEvents["event"];
	bool eventFired = false;
	QObject::connect(event, &Event::fired, [&eventFired]() { eventFired = true; });

	mTimeline.setTimestamp(1000);
	event->check();
	ASSERT_TRUE(eventFired);

	ASSERT_TRUE(mVariables.contains("x"));
	ASSERT_TRUE(mVariables["x"].type() == QVariant::Int);
	ASSERT_EQ(mVariables["x"].toInt(), 123);
}

TEST_F(ConstraintsParserTests, communicationTest)
{
	// This program will iteratively add 2 to 'counter', then subtract 1, then again add 2 and so on...
	const QString xml =
			"<constraints>"
			"	<timelimit value=\"2000\"/>"
			"	<event id=\"Set Initial Value\" settedUpInitially=\"true\">"
			"		<condition>"
			"			<timer timeout=\"0\"/>"
			"		</condition>"
			"		<triggers>"
			"			<trigger>"
			"				<setter name=\"counter\"><int value=\"0\"/></setter>"
			"			</trigger>"
			"			<trigger>"
			"				<setUp id=\"Increment 2\" value=\"0\"/>"
			"			</trigger>"
			"		</triggers>"
			"	</event>"
			"	<event id=\"Check Done\" settedUpInitially=\"true\">"
			"		<condition>"
			"			<not>"
			"				<less>"
			"					<variableValue name=\"counter\"/>"
			"					<int value=\"10\"/>"
			"				</less>"
			"			</not>"
			"		</condition>"
			"		<trigger>"
			"			<success/>"
			"		</trigger>"
			"	</event>"
			"	<event id=\"Increment 2\" settedUpInitially=\"true\" dropsOnFire=\"false\">"
			"		<condition>"
			"			<timer timeout=\"1\"/>"
			"		</condition>"
			"		<triggers>"
			"			<setter name=\"counter\">"
			"				<sum>"
			"					<variableValue name=\"counter\"/>"
			"					<int value=\"2\"/>"
			"				</sum>"
			"			</setter>"
			"			<setUp id=\"Decrement 1\" />"
			"			<drop id=\"Increment 2\" />"
			"		</triggers>"
			"	</event>"
			"	<event id=\"Decrement 1\" settedUpInitially=\"true\">"
			"		<condition>"
			"			<timer timeout=\"1\"/>"
			"		</condition>"
			"		<triggers>"
			"			<setter name=\"counter\">"
			"				<sum>"
			"					<variableValue name=\"counter\"/>"
			"					<int value=\"-1\"/>"
			"				</sum>"
			"			</setter>"
			"			<setUp id=\"Increment 2\" />"
			"		</triggers>"
			"	</event>"
			"</constraints>";
	ASSERT_TRUE(mParser.parse(xml));
	ASSERT_EQ(mEvents.count(), 5);

	QMap<QString, int> fireCounters;

	for (const QString &eventId : mEvents.keys()) {
		Event * const event = mEvents[eventId];
		ASSERT_NE(event, nullptr);
		fireCounters[eventId] = 0;
		QObject::connect(event, &Event::fired, [&fireCounters, eventId]() { ++fireCounters[eventId]; });
		if (event->isAliveInitially()) {
			event->setUp();
		}
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

int TestObjectA::intProperty() const
{
	return mIntValue;
}

QString TestObjectA::stringProperty() const
{
	return mStringValue;
}

QRect TestObjectA::rectProperty() const
{
	return mRectValue;
}

QPoint TestObjectA::pointProperty() const
{
	return mPointValue;
}

QList<int> TestObjectA::intListProperty() const
{
	return mIntListValue;
}

void TestObjectA::setIntProperty(int value)
{
	mIntValue = value;
}

void TestObjectA::setStringProperty(const QString &value)
{
	mStringValue = value;
}

void TestObjectA::setRectProperty(const QRect &value)
{
	mRectValue = value;
}

void TestObjectA::setPointProperty(const QPoint &value)
{
	mPointValue = value;
}

void TestObjectA::setIntListProperty(const QList<int> &value)
{
	mIntListValue = value;
}

TestObjectA *TestObjectB::otherObject()
{
	return &mOtherObject;
}
