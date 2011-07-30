#pragma once

#include <QtCore/QDir>
#include <QtCore/QList>

#include "../toolPluginInterface/toolPluginInterface.h"
#include "../toolPluginInterface/customToolInterface.h"
#include "../toolPluginInterface/customizationInterface.h"
#include "../toolPluginInterface/pluginConfigurator.h"
#include "../dialogs/preferencesPages/page.h"

namespace qReal {

class ToolPluginManager
{
public:
	explicit ToolPluginManager(QObject *parent = NULL);
	virtual ~ToolPluginManager();

	void init(PluginConfigurator const &configurator);
	QList<CustomToolInterface::ActionInfo> actions() const;
	QList<QPair<QString, PreferencesPage *> > preferencesPages() const;
	CustomizationInterface *customizer() const;

private:
	QDir mPluginsDir;

	QList<ToolPluginInterface *> mPlugins;
};

}
