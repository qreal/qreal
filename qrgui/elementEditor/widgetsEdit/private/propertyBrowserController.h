#pragma once

#include <QtCore/QObject>

#include "../../qtpropertybrowser/src/QtTreePropertyBrowser.h"
#include "../../qtpropertybrowser/src/QtVariantPropertyManager"
#include "../../qtpropertybrowser/src/QtVariantProperty.h"
#include "../../qtpropertybrowser/src/QtVariantEditorFactory"

namespace qReal
{
namespace widgetsEdit
{

class PropertyManager;

class PropertyBrowserController
{
public:
	PropertyBrowserController(QtTreePropertyBrowser *browser);
	void setController(PropertyManager *manager);

private:
	void clearExistingManager();

	QtTreePropertyBrowser *mBrowser;
	QtVariantEditorFactory *mFactory;
	PropertyManager *mCurrentManager;
};

}
}
