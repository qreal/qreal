#pragma once

#include <QtCore/QDir>
#include <QtCore/QList>
#include <QtCore/QPluginLoader>

#include "../toolPluginInterface/toolPluginInterface.h"
#include "../toolPluginInterface/customizer.h"
#include "../toolPluginInterface/pluginConfigurator.h"
#include "../dialogs/preferencesPages/page.h"

namespace qReal {

class ToolPluginManager
{
public:
	explicit ToolPluginManager(QObject *parent = NULL);
	virtual ~ToolPluginManager();

	void init(PluginConfigurator const &configurator);
	void updateSettings();
	QList<ActionInfo> actions() const;
	QList<QPair<QString, PreferencesPage *> > preferencesPages() const;
	Customizer const *customizer() const;

private:
	QDir mPluginsDir;

	QList<ToolPluginInterface *> mPlugins;
	QList<QPluginLoader *> mLoaders;
	Customizer const mCustomizer;  // Has ownership
};

}
