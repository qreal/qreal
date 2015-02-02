#pragma once

#include <QtCore/QObject>
#include <QtGui/QIcon>

#include "qrgui/mainWindow/mainWindowDockInterface.h"

namespace qReal {

/// Customization class for configuring QReal's main GUI module
class Customizer
{
public:
	virtual ~Customizer() {}

	/// Tells if we should show the button of opening the interpreted diagram or not
	virtual bool showInterpeterButton() const
	{
		return true;
	}

	/// Allows to customize main window's title from plugin
	virtual QString windowTitle() const
	{
		return "QReal";
	}

	/// Customizes QReal's icon
	virtual QIcon applicationIcon() const
	{
		return QIcon();
	}

	/// Customizes QReal`s logo in large format
	virtual QImage applicationLogo() const
	{
		return QImage(":/mainWindow/images/kroki6.png");
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

	/// Customizes default examples directory for current system.
	/// Empty string as result means that the system doesn`t have examples
	virtual QString examplesDirectory() const
	{
		return QString();
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

	virtual QString userPaletteTitle() const
	{
		return QObject::tr("Existing connections");
	}

	virtual QString userPaletteDescription() const
	{
		return QObject::tr("Elements from this group exist"\
				" for reusing all created connections");
	}
};

}
