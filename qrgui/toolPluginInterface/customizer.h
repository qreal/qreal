#pragma once

#include <QtCore/QObject>
#include <QtGui/QIcon>

namespace qReal {

class Customizer
{
public:
	virtual bool showLogicalModelExplorer() const
	{
		return true;
	}

	virtual QString windowTitle() const
	{
		return "";
	}

	virtual QIcon applicationIcon() const
	{
		return QIcon();
	}

	virtual bool showConnectionRelatedMenus() const
	{
		return false;
	}
};

}
