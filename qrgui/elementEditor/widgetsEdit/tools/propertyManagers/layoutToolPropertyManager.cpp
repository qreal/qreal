#include "layoutToolPropertyManager.h"

using namespace qReal::widgetsEdit;

LayoutToolPropertyManager::LayoutToolPropertyManager(LayoutTool *LayoutTool)
	: ToolPropertyManager(LayoutTool)
{
}

QVariant LayoutToolPropertyManager::propertyValue(const QString &name)
{
	return ToolPropertyManager::propertyValue(name);
}
