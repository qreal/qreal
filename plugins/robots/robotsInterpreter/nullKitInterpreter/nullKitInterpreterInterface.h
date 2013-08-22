#pragma once

#include <QtCore/QObject>

#include <robotsInterpreterCore/kitPluginInterface.h>

namespace nullKitInterpreter {

class NullKitInterpreterInterface : public QObject, public robotsInterpreterCore::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(robotsInterpreterCore::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "nullKitInterpreter.NullKitInterpreterInterface")

public:
	// Override.
	virtual QString kitId() const;

	// Override.
	// Transfers ownership.
	virtual QWidget *settingsWidget() const;

	// Override.
	// Transfers ownership.
//	virtual QList<QWidget *> toolWidgets() const;

	// Override.
//	virtual QList<robotsInterpreterCore::SensorId> sensorIds() const;

	// Override.
	virtual qReal::IdList specificBlocks() const;

	// Override.
	virtual qReal::IdList unsupportedBlocks() const;
};

}
