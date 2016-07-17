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

#pragma once

#include <QtCore/QRect>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <src/engine/constraints/details/constraintsParser.h>
#include "support/testTimeline.h"
#include "support/testObject.h"

namespace qrTest {
namespace robotsTests {
namespace commonTwoDModelTests {

class TestObjectA : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int intProperty READ intProperty WRITE setIntProperty)
	Q_PROPERTY(QString stringProperty READ stringProperty WRITE setStringProperty)
	Q_PROPERTY(QRect rectProperty READ rectProperty WRITE setRectProperty)
	Q_PROPERTY(QPoint pointProperty READ pointProperty WRITE setPointProperty)
	Q_PROPERTY(QList<int> intListProperty READ intListProperty WRITE setIntListProperty)

public:
	int intProperty() const;
	QString stringProperty() const;
	QRect rectProperty() const;
	QPoint pointProperty() const;
	QList<int> intListProperty() const;

	void setIntProperty(int value);
	void setStringProperty(const QString &value);
	void setRectProperty(const QRect &value);
	void setPointProperty(const QPoint &value);
	void setIntListProperty(const QList<int> &value);

private:
	int mIntValue;
	QString mStringValue;
	QRect mRectValue;
	QPoint mPointValue;
	QList<int> mIntListValue;
};

class TestObjectB : public QObject
{
	Q_OBJECT
	Q_PROPERTY(TestObjectA *otherObject READ otherObject)

public:
	TestObjectA *otherObject();

private:
	TestObjectA mOtherObject;
};

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
