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
