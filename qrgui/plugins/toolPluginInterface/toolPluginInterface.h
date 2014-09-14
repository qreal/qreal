#pragma once

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtCore/QList>
#include <QtWidgets/QAction>

#include <qrrepo/repoControlInterface.h>
#include "preferencesDialog/preferencesPage.h"

#include "customizer.h"
#include "pluginConfigurator.h"
#include "actionInfo.h"
#include "hotKeyActionInfo.h"
#include "projectConverter.h"

namespace qReal {

/// Base class for tool plugins, actually an interface with default implementations.
/// This is all that qrgui knows about tool plugins. Every tool plugin shall have
/// a class derived from this and override needed methods. Default implementations
/// are provided as "do nothing" behavior, so if a plugin doesn't need some features,
/// it shouldn't care about them.
class ToolPluginInterface
{
public:

	/// Shall be overriden to return customizer that allows to change window title,
	/// show/hide certain GUI elements and so on.
	virtual Customizer* customizationInterface()
	{
		return NULL;
	}

	/// Shall be overriden to return QAction or QMenu objects along with where to put them in
	/// main window. Menus can contains more actions. These actions shall be connected
	/// to slots of a plugin, so when user clicks on an action, some code in plugin gets executed
	virtual QList<ActionInfo> actions()
	{
		return QList<ActionInfo>();
	}

	/// Shall be overriden in concrete plugin to initialize itself.
	/// @param configurator Object that contains all needed information for a plugin, like refs to models.
	virtual void init(PluginConfigurator const &configurator)
	{
		Q_UNUSED(configurator);
	}

	/// Returns preferences page along with its name if plugin has any, NULL otherwise.
	/// Ownership is passed to the caller.
	virtual QPair<QString, gui::PreferencesPage *> preferencesPage()
	{
		return QPair<QString, gui::PreferencesPage *>(QString(), nullptr);
	}

	/// Shall be overridden to return QAction instances for their customization in
	/// hot key manager.
	virtual QList<HotKeyActionInfo> hotKeyActions()
	{
		return QList<HotKeyActionInfo>();
	}

	/// Returns a list of project converters provided by this plugin.
	/// Convereters are applied to save containing corresponding diagrams
	/// sequentially moving from save`s version to system one.
	/// If no converter provided for some version than it is considered that
	/// no modifications required for such save.
	virtual QList<ProjectConverter> projectConverters()
	{
		return QList<ProjectConverter>();
	}

	/// Returns a list of pathes to ini files that contain default settings in key-value format
	/// for this plugin. If there are collisions between keys in different plugins then selected
	/// value is undefined. However engine`s default settings can be 'overloaded' by plugin ones.
	/// Default implementation returns empty list.
	virtual QStringList defaultSettingsFiles()
	{
		return QStringList();
	}
};

}

Q_DECLARE_INTERFACE(qReal::ToolPluginInterface, "ru.tepkom.QReal.ToolPluginInterface/0.2")
