#pragma once

#include <QtCore/QObject>
#include <QtGui/QIcon>

#include "../mainwindow/mainWindowDockInterface.h"
#include "../view/sceneCustomizationInterface.h"

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

	/// Customizes docks placement and visibility. Default implementation does nothing
	virtual void customizeDocks(gui::MainWindowDockInterface *dockInterface)
	{
		Q_UNUSED(dockInterface)
	}

	/// Customizes QReal scene. Default implementation does nothing
	virtual void customizeScene(SceneCustomizationInterface *sceneCustomizer)
	{
		Q_UNUSED(sceneCustomizer)
	}
};

}
