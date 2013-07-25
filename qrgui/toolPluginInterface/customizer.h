#pragma once

#include <QtCore/QObject>
#include <QtGui/QIcon>

#include "../mainwindow/mainWindowDockInterface.h"

namespace qReal {

/// Customization class for configuring QReal's main GUI module
class Customizer
{
public:
	/// Tells if we should show the button of opening the interpreted diagram or not
	virtual bool showInterpeterButton() const
	{
		return true;
	}

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

	/// Customizes product version
	virtual QString productVersion() const
	{
		return "1.0.0";
	}

	/// Customizes text in about dialog
	virtual QString aboutText() const
	{
		return "<b>QReal<b><br><br><a href=\"http://qreal.ru/\">http://qreal.ru/</a>";
	}

	/// Tells if we should show trace connections menu or not
	virtual bool showConnectionRelatedMenus() const
	{
		return false;
	}

	virtual bool showUsagesRelatedMenus() const
	{
		return false;
	}

	/// Customizes docks placement and visibility. Default implementation does nothing
	virtual void customizeDocks(gui::MainWindowDockInterface *dockInterface)
	{
		Q_UNUSED(dockInterface)
	}

	virtual QString addConnectionMenuName() const
	{
		return QObject::tr("Add connection");
	}

	virtual QString deleteConnectionMenuName() const
	{
		return QObject::tr("Disconnect");
	}

	virtual QString addUsageMenuName() const
	{
		return QObject::tr("Add usage");
	}

	virtual QString deleteUsageMenuName() const
	{
		return QObject::tr("Delete usage");
	}

	virtual QString forwardConnectionMenuName() const
	{
		return QObject::tr("Forward connection");
	}

	virtual QString backwardConnectionMenuName() const
	{
		return QObject::tr("Backward connection");
	}

	virtual QString forwardUsageMenuName() const
	{
		return QObject::tr("Uses");
	}

	virtual QString backwardUsageMenuName() const
	{
		return QObject::tr("Used in");
	}

	virtual bool showExpandConnectionAction() const
	{
		return false;
	}

	virtual QString expandConnectionActionText() const
	{
		return QObject::tr("Expand connection");
	}

	virtual QString collapseConnectionActionText() const
	{
		return QObject::tr("Collapse connection");
	}
};

}
