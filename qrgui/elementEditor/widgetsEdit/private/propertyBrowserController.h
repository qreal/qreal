#pragma once

#include <QtCore/QObject>

#include <thirdparty/qtpropertybrowser/src/QtTreePropertyBrowser.h>
#include <thirdparty/qtpropertybrowser/src/QtVariantPropertyManager>
#include <thirdparty/qtpropertybrowser/src/QtVariantProperty.h>
#include <thirdparty/qtpropertybrowser/src/QtVariantEditorFactory>

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
