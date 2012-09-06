#include "rootPropertyManager.h"

using namespace qReal::widgetsEdit;

RootPropertyManager::RootPropertyManager(Root *root)
	: LayoutToolPropertyManager(root)
{
}

QVariant RootPropertyManager::propertyValue(const QString &name)
{
	return LayoutToolPropertyManager::propertyValue(name);
}
