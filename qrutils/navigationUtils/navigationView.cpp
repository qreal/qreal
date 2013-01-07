#include "navigationView.h"

using namespace navigation;

NavigationView::NavigationView(
		QMap<NavigationPageInterface*, QWidget *> const &pages
		, NavigationController *controller, QWidget *parent)
	: QWidget(parent), mPages(pages), mController(controller)
	, mCurrentPage(NULL), mMenu(NULL)
{
	initialize();
}

QWidget *NavigationView::initNavigationArea()
{
	mMenuLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	QWidget *mainArea = new QWidget;
	mMenuLayout->insertWidget(1, mainArea);
	setLayout(mMenuLayout);
	return mainArea;
}

NavigationMenu *NavigationView::menu() const
{
	return mMenu;
}

NavigationPageInterface *NavigationView::currentPage() const
{
	return mCurrentPage;
}

void NavigationView::enableMenuSupport()
{
	if (mMenu) {
		return;
	}
	mMenu = new NavigationMenu;
	mMenuLayout->insertWidget(0, mMenu);
}

void NavigationView::disableMenuSupport()
{
	if (mMenu) {
		delete mMenu;
	}
	mMenu = NULL;
}

void NavigationView::initialize()
{
	mLayout = new QStackedLayout;
	initControllerInteraction();
	QWidget *navigationArea = initNavigationArea();
	navigationArea->setLayout(mLayout);
	changePage(mController->defaultPage());
}

void NavigationView::initControllerInteraction()
{
	connect(mController, SIGNAL(pageChanged(NavigationPageInterface*))
			, this, SLOT(changePage(NavigationPageInterface*)));
}

void NavigationView::deinitControllerInteraction()
{
	disconnect(this, SLOT(changePage(NavigationPageInterface*)));
}

NavigationController *NavigationView::controller() const
{
	return mController;
}

void NavigationView::setController(NavigationController *controller)
{
	deinitControllerInteraction();
	mController = controller;
	initControllerInteraction();
}

void NavigationView::changePage(NavigationPageInterface *page)
{
	if (!mPages.contains(page)) {
		return;
	}
	NavigationPageWithMenu *menuPage = dynamic_cast<NavigationPageWithMenu *>(page);
	if (menuPage) {
		if (mMenu) {
			mMenu->setContent(menuPage->menuContent());
		}
	} else {
		mMenu->hide();
	}
	QWidget *widget = mPages[page];
	if (!widget || mLayout->currentWidget() == widget) {
		return;
	}
	if (!alreadyAdded(widget)) {
		mLayout->addWidget(widget);
	}
	mLayout->setCurrentWidget(widget);
	if (mCurrentPage) {
		mCurrentPage->onHidden();
	}
	mCurrentPage = page;
	page->onShown(mController->state());
}

bool NavigationView::alreadyAdded(QWidget *widget)
{
	for (int i = 0; i < mLayout->count(); ++i) {
		if (mLayout->widget(i) == widget) {
			return true;
		}
	}
	return false;
}
