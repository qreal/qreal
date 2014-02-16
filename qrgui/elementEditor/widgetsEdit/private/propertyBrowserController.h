#pragma once

#include <QtCore/QObject>

#include <thirdparty/qtpropertybrowser/src/qttreepropertybrowser.h>
#include <thirdparty/qtpropertybrowser/src/qtvariantproperty.h>

namespace qReal {
namespace widgetsEdit {

class PropertyManager;

class PropertyBrowserController
{
public:
	explicit PropertyBrowserController(QtTreePropertyBrowser *browser);
	void setController(PropertyManager *manager);

private:
	void clearExistingManager();

	QtTreePropertyBrowser *mBrowser;
	QtVariantEditorFactory *mFactory;
	PropertyManager *mCurrentManager;
};

}
}
