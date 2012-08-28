#pragma once

#include "../layoutTool.h"
#include "toolPropertyManager.h"

namespace Ui
{
namespace WidgetsEdit
{

class LayoutToolPropertyManager : public ToolPropertyManager
{
public:
	LayoutToolPropertyManager(LayoutTool *layoutTool);

	virtual QVariant propertyValue(const QString &name);

};

}
}
