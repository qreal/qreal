#include "kitPluginManager.h"

using namespace robotsInterpreterCore;

QList<QString> KitPluginManager::kitNames() const
{
	return QList<QString>();
}

QWidget *KitPluginManager::kitSpecificSettingsWidget(QString const &kitName) const
{
	Q_UNUSED(kitName);

	return NULL;
}
