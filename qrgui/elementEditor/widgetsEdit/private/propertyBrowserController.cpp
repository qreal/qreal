#include "propertyBrowserController.h"
#include "../tools/propertyManagers/property.h"

using namespace Ui::WidgetsEdit;

PropertyBrowserController::PropertyBrowserController(QtTreePropertyBrowser *browser)
	: mBrowser(browser)
	, mFactory(new QtVariantEditorFactory)
	, mCurrentManager(NULL)
{
}

void PropertyBrowserController::setController(PropertyManagerBase *manager)
{
	clearExstingManager();
	mCurrentManager = manager;
	mCurrentManager->onSelect();
	mBrowser->setFactoryForManager(mCurrentManager->qtPropertyManager(), mFactory);
	QListIterator<Property *> it = mCurrentManager->propertiesIterator();
	while (it.hasNext()) {
		mBrowser->addProperty(it.next()->qtProperty());
	}
}

void PropertyBrowserController::clearExstingManager()
{
	mBrowser->clear();
	if (mCurrentManager) {
		mBrowser->unsetFactoryForManager(mCurrentManager->qtPropertyManager());
	}
}
