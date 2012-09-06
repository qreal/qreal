#pragma once

#include "../widget.h"
#include "layoutToolPropertyManager.h"

namespace qReal
{
namespace widgetsEdit
{

class WidgetPropertyManager : public LayoutToolPropertyManager
{
public:
	WidgetPropertyManager(Widget *widget);

	virtual QVariant propertyValue(const QString &name);

};

}
}
