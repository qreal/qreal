#pragma once

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtWidgets/QWidget>

#include "block.h"
#include "sensorId.h"

namespace robotsInterpreterCore {

class KitPluginInterface
{
public:
	virtual QString kitName() const = 0;

	// Transfers ownership.
	virtual QWidget *settingsWidget() const = 0;

	// Transfers ownership.
	virtual QList<QWidget *> toolWidgets() const = 0;

	virtual QList<SensorId> sensorIds() const = 0;
};

}
