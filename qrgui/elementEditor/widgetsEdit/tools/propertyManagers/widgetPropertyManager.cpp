#include "widgetPropertyManager.h"

using namespace qReal::widgetsEdit;

WidgetPropertyManager::WidgetPropertyManager(Widget *widget)
	: LayoutToolPropertyManager(widget)
{
}

QVariant WidgetPropertyManager::propertyValue(const QString &name)
{
	return LayoutToolPropertyManager::propertyValue(name);
}
