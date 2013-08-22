#include "mockKitPlugin1Interface.h"

using namespace qrTest::mockKitPlugin1;

QString MockKitPlugin1Interface::kitName() const
{
	return "MockKitPlugin1";
}

QWidget *MockKitPlugin1Interface::settingsWidget() const
{
	return NULL;
}

QList<QWidget *> MockKitPlugin1Interface::toolWidgets() const
{
	return QList<QWidget *>();
}

QList<robotsInterpreterCore::SensorId> MockKitPlugin1Interface::sensorIds() const
{
	return QList<robotsInterpreterCore::SensorId>();
}
