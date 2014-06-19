#include "propertyBrowserController.h"
#include <qrgui/elementEditor/widgetsEdit/tools/property.h>
#include <qrgui/elementEditor/widgetsEdit/tools/propertyManager.h>

using namespace qReal::widgetsEdit;

PropertyBrowserController::PropertyBrowserController(QtTreePropertyBrowser *browser)
	: mBrowser(browser)
	, mFactory(new QtVariantEditorFactory)
	, mCurrentManager(NULL)
{
}

void PropertyBrowserController::setController(PropertyManager *manager)
{
	clearExistingManager();
	mCurrentManager = manager;
	mCurrentManager->onSelect();
	mBrowser->setFactoryForManager(mCurrentManager->qtPropertyManager(), mFactory);
	QListIterator<Property *> it = mCurrentManager->userPropertiesIterator();
	while (it.hasNext()) {
		mBrowser->addProperty(it.next()->qtProperty());
	}
}

void PropertyBrowserController::clearExistingManager()
{
	mBrowser->clear();
	if (mCurrentManager) {
		mBrowser->unsetFactoryForManager(mCurrentManager->qtPropertyManager());
	}
}
