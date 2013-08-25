#pragma once

#include <QtCore/QObject>

#include <robotsInterpreterCore/kitPluginInterface.h>

namespace qrTest {
namespace mockKitPlugin1 {

class MockKitPlugin1Interface : public QObject, public robotsInterpreterCore::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(robotsInterpreterCore::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "mockKitPlugin1.MockKitPlugin1Interface")

public:
	// Override.
	virtual QString kitId() const;

	// Override.
	// Transfers ownership.
	virtual QWidget *settingsWidget() const;

	// Override.
	virtual qReal::IdList unsupportedBlocks() const;

	// Override.
	// Transfers ownership.
//	virtual QList<QWidget *> toolWidgets() const;

	// Override.
//	virtual QList<robotsInterpreterCore::robotModel::SensorId> sensorIds() const;
};

}
}
