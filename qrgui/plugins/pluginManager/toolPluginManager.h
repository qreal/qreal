#pragma once

#include <QtCore/QDir>
#include <QtCore/QList>
#include <QtCore/QPluginLoader>
#include <QtWidgets/QAction>

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QMap>

#include <qrutils/pluginManagers/pluginManager.h>
#include <qrgui/preferencesDialog/preferencesPage.h>

#include <plugins/toolPluginInterface/toolPluginInterface.h>
#include <plugins/toolPluginInterface/customizer.h>
#include <plugins/toolPluginInterface/pluginConfigurator.h>
#include <plugins/toolPluginInterface/hotKeyActionInfo.h>
#include <plugins/toolPluginInterface/systemEvents.h>

#include "plugins/pluginManager/pluginsManagerDeclSpec.h"

namespace qReal {

class QRGUI_PLUGINS_MANAGER_EXPORT ToolPluginManager
{
public:
	ToolPluginManager();
	~ToolPluginManager();

	void init(PluginConfigurator const &configurator);
	void updateSettings();
	QList<ActionInfo> actions() const;
	QList<QPair<QString, gui::PreferencesPage *> > preferencesPages() const;

	/// Returns a list of hotkeys specified in plugins.
	QList<HotKeyActionInfo> hotKeyActions() const;

	/// Returns a multimap of project conveters to editors whoose diagrams they convert.
	/// @warning The result is obtained each time from scratch when you call this method so better to memorize it.
	QMultiMap<QString, ProjectConverter> projectConverters() const;

	/// Returns GUI customizer object. In each QReal's instance there should be only one plugin with
	/// valid customizationInterface(): robots plugin for QReal:Robots, ubiq plugin for QReal:Ubiq etc.
	/// If there're more than one plugin of that kind, it is not specified which one will be used.
	Customizer *customizer() const;

	/// Notifies plugins about change of currently open tab. If last tab is closed or current
	/// tab is not diagram at all (text editor, for example), rootElementId is Id()
	/// @param rootElementId Id of the tab which became active after change, if applicable. If not, Id().
	void activeTabChanged(Id const & rootElementId);

private:
	/// Asks plugins for custom default settings and merges them with engine`s ones.
	void loadDefaultSettings();

	QDir mPluginsDir;

	QList<ToolPluginInterface *> mPlugins;

	/// An object and that is used to customize GUI with values from plugins
	Customizer mCustomizer;

	/// Common part of all plugin managers
	PluginManager mPluginManager;

	SystemEvents *mSystemEvents;
};

}
