#pragma once

#include <QtCore/QObject>
#include <QtGui/QIcon>

namespace qReal {

class CustomizationInterface
{
public:
	virtual bool showLogicalModelExplorer() const = 0;
	virtual QString windowTitle() const = 0;
	virtual QIcon applicationIcon() const = 0;
};

}
