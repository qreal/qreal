#pragma once

#include "robotsSettingsPageExtensionsInterface.h"

namespace robotsInterpreterCore {

class KitPluginManager : public RobotsSettingsPageExtensionsInterface
{
public:
	// Override.
	virtual QList<QString> kitNames() const;

	// Override.
	// Transfers ownership.
	virtual QWidget *kitSpecificSettingsWidget(QString const &kitName) const;
};

}

