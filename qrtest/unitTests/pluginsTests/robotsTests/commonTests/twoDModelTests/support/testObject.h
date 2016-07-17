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

#include <QtCore/QObject>

namespace qrTest {
namespace robotsTests {
namespace commonTwoDModelTests {

/// Just a regular QObject mock for testing binding to Qt properties.
class TestObject : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int intProperty READ intProperty WRITE setIntProperty)
	Q_PROPERTY(qreal doubleProperty READ doubleProperty WRITE setDoubleProperty)
	Q_PROPERTY(QString stringProperty READ stringProperty WRITE setStringProperty)

public:
	int intProperty() const;
	void setIntProperty(int value);

	qreal doubleProperty() const;
	void setDoubleProperty(qreal value);

	QString stringProperty() const;
	void setStringProperty(const QString &value);

private:
	int mIntValue;
	qreal mDoubleValue;
	QString mStringValue;
};

}
}
}
