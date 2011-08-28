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
	void updateSettings();
	QList<CustomToolInterface::ActionInfo> actions() const;
	QList<QPair<QString, PreferencesPage *> > preferencesPages() const;
	CustomizationInterface *customizer() const;

private:

	class DummyCustomizer : public CustomizationInterface
	{
		virtual bool showLogicalModelExplorer() const
		{
			return true;
		}

		virtual QString windowTitle() const
		{
			return QString();
		}

		virtual QIcon applicationIcon() const
		{
			return QIcon();
		}
	};

	QDir mPluginsDir;

	QList<ToolPluginInterface *> mPlugins;
	DummyCustomizer *mCustomizer;  // Has ownership
};

}
