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
    // efimefim change back before pull request
	virtual bool showConnectionRelatedMenus() const
	{
        return true;
	}

    /// Tells if we should show library related menu or not
    virtual bool showLibraryRelatedMenus() const
    {
        return true;
    }
};

}
