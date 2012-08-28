#include "rootPropertyManager.h"

using namespace Ui::WidgetsEdit;

RootPropertyManager::RootPropertyManager(Root *root)
	: LayoutToolPropertyManager(root)
{
}

QVariant RootPropertyManager::propertyValue(const QString &name)
{
	return LayoutToolPropertyManager::propertyValue(name);
}
