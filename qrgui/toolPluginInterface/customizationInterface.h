#pragma once

#include <QtCore/QObject>

namespace qReal {

class CustomizationInterface
{
public:
	virtual bool showLogicalModelExplorer() const = 0;
	virtual QString windowTitle() const = 0;
};

}
