#pragma once

#include "../root.h"
#include "layoutToolPropertyManager.h"

namespace Ui
{
namespace WidgetsEdit
{

class RootPropertyManager : public LayoutToolPropertyManager
{
public:
	RootPropertyManager(Root *root);

	virtual QVariant propertyValue(const QString &name);

};

}
}
