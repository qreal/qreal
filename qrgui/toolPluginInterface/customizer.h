#pragma once

#include <QtCore/QObject>
#include <QtGui/QIcon>

#include "../mainwindow/mainWindowDockInterface.h"

namespace qReal {

/// Customization class for configuring QReal's main GUI module
class Customizer
{
public:
	/// Allows to customize main window's title from plugin
	virtual QString windowTitle() const
	{
		return "";
	}

	/// Customizes QReal's icon
	virtual QIcon applicationIcon() const
	{
		return QIcon();
	}

	/// Tells if we should show trace connections menu or not
	virtual bool showConnectionRelatedMenus() const
	{
		return false;
	}

	/// Customizes docks placement and visibility. Default implementation does nothing
	virtual void customizeDocks(gui::MainWindowDockInterface *dockInterface)
	{
		Q_UNUSED(dockInterface)
	}
};

}
