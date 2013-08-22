#pragma once

#include <QtCore/QObject>

#include <robotsInterpreterCore/kitPluginInterface.h>

namespace qrTest {
namespace mockKitPlugin2 {

class MockKitPlugin2Interface : public QObject, public robotsInterpreterCore::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(robotsInterpreterCore::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "mockKitPlugin2.MockKitPlugin1Interface")

public:
	// Override.
	virtual QString kitName() const;

	// Override.
	// Transfers ownership.
	virtual QWidget *settingsWidget() const;

	// Override.
	// Transfers ownership.
	virtual QList<QWidget *> toolWidgets() const;

	// Override.
	virtual QList<robotsInterpreterCore::SensorId> sensorIds() const;
};

}
}
