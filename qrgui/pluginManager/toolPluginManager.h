#pragma once

#include <QtCore/QDir>
#include <QtCore/QList>

#include "../toolPluginInterface/toolPluginInterface.h"
#include "../toolPluginInterface/customToolInterface.h"
#include "../toolPluginInterface/pluginConfigurator.h"

namespace qReal {

class ToolPluginManager
{
public:
	explicit ToolPluginManager(QObject *parent = NULL);
	virtual ~ToolPluginManager();

	void init(PluginConfigurator const &configurator);
	QList<CustomToolInterface::ActionInfo> actions() const;

private:
	QDir mPluginsDir;

	QList<ToolPluginInterface *> mPlugins;
};

}
