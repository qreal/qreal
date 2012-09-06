#pragma once

#include "../root.h"
#include "layoutToolPropertyManager.h"

namespace qReal
{
namespace widgetsEdit
{

class RootPropertyManager : public LayoutToolPropertyManager
{
public:
	RootPropertyManager(Root *root);

	virtual QVariant propertyValue(const QString &name);

};

}
}
