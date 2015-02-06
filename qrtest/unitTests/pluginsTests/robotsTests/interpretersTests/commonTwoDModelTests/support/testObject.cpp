#include "testObject.h"

using namespace qrTest::robotsTests::commonTwoDModelTests;

int TestObject::intProperty() const
{
	return mIntValue;
}

void TestObject::setIntProperty(int value)
{
	mIntValue = value;
}

qreal TestObject::doubleProperty() const
{
	return mDoubleValue;
}

void TestObject::setDoubleProperty(qreal value)
{
	mDoubleValue = value;
}

QString TestObject::stringProperty() const
{
	return mStringValue;
}

void TestObject::setStringProperty(const QString &value)
{
	mStringValue = value;
}
