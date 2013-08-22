#include "mockKitPlugin2Interface.h"

using namespace qrTest::mockKitPlugin2;

QString MockKitPlugin2Interface::kitName() const
{
	return "MockKitPlugin2";
}

QWidget *MockKitPlugin2Interface::settingsWidget() const
{
	return NULL;
}

QList<QWidget *> MockKitPlugin2Interface::toolWidgets() const
{
	return QList<QWidget *>();
}

QList<robotsInterpreterCore::SensorId> MockKitPlugin2Interface::sensorIds() const
{
	return QList<robotsInterpreterCore::SensorId>();
}
