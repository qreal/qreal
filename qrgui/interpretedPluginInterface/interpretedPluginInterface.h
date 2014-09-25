#pragma once

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtCore/QList>
#include <QtWidgets/QAction>

#include <qrrepo/repoControlInterface.h>
#include <qrrepo/logicalRepoApi.h>
#include "dialogs/preferencesPages/preferencesPage.h"

#include "toolPluginInterface/customizer.h"
#include "toolPluginInterface/pluginConfigurator.h"
#include "toolPluginInterface/actionInfo.h"
#include "toolPluginInterface/hotKeyActionInfo.h"
#include "toolPluginInterface/projectConverter.h"

namespace qReal {

/// Base class for interpreted plugins.
class InterpretedPluginInterface
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
	/// @param configurator - Object that contains all needed information for a plugin, like refs to models.
	/// @param metamodelRepoApi - Object that contains information about metamodel
	virtual void init(
			PluginConfigurator const &configurator
			, qrRepo::LogicalRepoApi &metamodelRepoApi
			)
	{
		Q_UNUSED(configurator);
		Q_UNUSED(metamodelRepoApi);
	}

	/// Returns preferences page along with its name if plugin has any, NULL otherwise.
	/// Ownership is passed to the caller.
	virtual QPair<QString, PreferencesPage *> preferencesPage()
	{
		return QPair<QString, PreferencesPage *>(QString(), NULL);
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

Q_DECLARE_INTERFACE(qReal::InterpretedPluginInterface, "ru.tepkom.QReal.InterpretedPluginInterface/0.2")
