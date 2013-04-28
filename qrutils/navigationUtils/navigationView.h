#pragma once

#include <QtCore/QMap>
#include <QtWidgets/QWidget>
#include <QtWidgets/QStackedLayout>

#include "../utilsDeclSpec.h"
#include "navigationController.h"
#include "navigationPageInterface.h"
#include "navigationMenu.h"
#include "navigationPageWithMenu.h"

namespace navigation
{

/// The base class for all widgets with navigation. Can be used as-is
class QRUTILS_EXPORT NavigationView : public QWidget
{
	Q_OBJECT

public:
	NavigationView(QMap<NavigationPageInterface*, QWidget *> const &pages
			, NavigationController *controller = NULL, QWidget *parent = NULL);
	virtual ~NavigationView();

	NavigationController *controller() const;
	void setController(NavigationController *controller);

protected:
	/// Inits and returns the widget where pages will be drawn.
	/// In default implementation this is the area under the menu on the top.
	virtual QWidget *initNavigationArea();
	/// Enables menu drawing when NavigationPageWithMenu becomes active
	/// (by default, menu support is disabled)
	void enableMenuSupport();
	/// Disables menu support and deletes menu.
	/// NOTE: In current implementation all the pages menu content will
	/// be removed too.
	void disableMenuSupport();
	inline NavigationMenu *menu() const;
	NavigationPageInterface *currentPage() const;

	QMap<NavigationPageInterface*, QWidget *> mPages;
	NavigationController *mController;

protected slots:
	virtual void changePage(NavigationPageInterface *page);

private:
	void initialize();
	void initControllerInteraction();
	void deinitControllerInteraction();
	bool alreadyAdded(QWidget *widget);

	QBoxLayout *mMenuLayout;
	QStackedLayout *mLayout;
	NavigationPageInterface *mCurrentPage;
	NavigationMenu *mMenu;
};

}
