#include "widgetPropertyManager.h"

using namespace Ui::WidgetsEdit;

WidgetPropertyManager::WidgetPropertyManager(Widget *widget)
	: LayoutToolPropertyManager(widget)
{
}

QVariant WidgetPropertyManager::propertyValue(const QString &name)
{
	return LayoutToolPropertyManager::propertyValue(name);
}
