#pragma once

#include <QtCore/QObject>
#include <QtGui/QIcon>

namespace qReal {

/// Customization class for configuring QReal's main GUI module
class Customizer
{
public:
	/// Tells if we should show logical model explorer or not
	virtual bool showLogicalModelExplorer() const
	{
		return true;
	}

	/// Tells if we should show the button of opening the interpreted diagram or not
	virtual bool showInterpeterButton() const
	{
		return false;
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

	/// Tells if we should show trace connections menu or not
	virtual bool showConnectionRelatedMenus() const
	{
		return false;
	}
};

}
