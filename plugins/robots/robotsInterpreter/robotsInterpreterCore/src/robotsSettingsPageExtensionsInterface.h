#pragma once

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtWidgets/QWidget>

namespace robotsInterpreterCore {

class RobotsSettingsPageExtensionsInterface
{
public:
	virtual QList<QString> kitIds() const = 0;

	// Transfers ownership.
	virtual QWidget *kitSpecificSettingsWidget(QString const &kitName) const = 0;
};

}
