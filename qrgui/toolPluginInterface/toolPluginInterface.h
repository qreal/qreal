#pragma once

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtCore/QList>
#include <QtWidgets/QAction>

#include <qrrepo/repoControlInterface.h>
#include "dialogs/preferencesPages/preferencesPage.h"

#include "toolPluginInterface/customizer.h"
#include "toolPluginInterface/pluginConfigurator.h"
#include "toolPluginInterface/actionInfo.h"
#include "toolPluginInterface/hotKeyActionInfo.h"

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

	/// Shall be overriden to return facade for access to plugin gui.
	virtual QObject* guiScriptFacade()
	{
		return NULL;
	}

	///Shall be overriden to return plugin name.
	virtual QString pluginName()
	{
		return "";
	}
};

}

Q_DECLARE_INTERFACE(qReal::ToolPluginInterface, "ru.tepkom.QReal.ToolPluginInterface/0.2")
