#pragma once

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtCore/QList>

#include "../../qrrepo/repoControlInterface.h"
#include "../dialogs/preferencesPages/preferencesPage.h"

#include "customizer.h"
#include "pluginConfigurator.h"
#include "actionInfo.h"

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

	/// Shall be overriden in plugin to react on application settings changes.
	/// Called when user clicks Ok or Apply in preferences dialog, for example.
	virtual void updateSettings()
	{
	}

	/// Shall be overriden to return QAction objects along with where to put them in
	/// main window. These actions shall be connected to slots of a plugin, so when user
	/// clicks on an action, some code in plugin gets executed
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
	virtual QPair<QString, PreferencesPage *> preferencesPage()
	{
		return QPair<QString, PreferencesPage *>(QString(), NULL);
	}

	/// Shall be overriden if plugin needs to do some action on closing of main window,
	/// like closing its own windows.
	// TODO: Properly rename it or remove at all.
	virtual void closeNeededWidget()
	{
	}

	/// Event that is sent to a plugin by GUI when user switches or closes a tab
	/// @param rootElementId Id of the root element of a new tab, whose diagram is shown
	virtual void activeTabChanged(Id const & rootElementId)
	{
		Q_UNUSED(rootElementId);
	}
};

}

Q_DECLARE_INTERFACE(qReal::ToolPluginInterface, "ru.tepkom.QReal.ToolPluginInterface/0.2")
