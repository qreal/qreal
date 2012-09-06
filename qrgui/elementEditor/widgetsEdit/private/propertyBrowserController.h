#pragma once

#include <QtCore/QObject>

#include "../tools/propertyManagers/propertyManagerBase.h"
#include "../../qtpropertybrowser/src/QtTreePropertyBrowser"
#include "../../qtpropertybrowser/src/QtVariantPropertyManager"
#include "../../qtpropertybrowser/src/QtVariantProperty"
#include "../../qtpropertybrowser/src/QtVariantEditorFactory"

namespace qReal
{
namespace widgetsEdit
{

class PropertyManagerBase;

class PropertyBrowserController
{
public:
	PropertyBrowserController(QtTreePropertyBrowser *browser);
	void setController(PropertyManagerBase *manager);

private:
	void clearExstingManager();

	QtTreePropertyBrowser *mBrowser;
	QtVariantEditorFactory *mFactory;
	PropertyManagerBase *mCurrentManager;
};

}
}
