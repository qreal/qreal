#pragma once

#include <QtCore/QObject>
#include <QtGui/QAction>
#include <QtGui/QMenu>

namespace qReal {

class ActionInfo {
public:
	ActionInfo(QAction * const action, QString const &toolbarName, QString const &menuName)
			: mAction(action),mMenu(NULL),
			mToolbarName(toolbarName), mMenuName(menuName),
			mIsAction(true)
	{
	}
	
	ActionInfo(QMenu * const menu, QString const &menuName)
			: mAction(NULL), mMenu(menu),
			mToolbarName(""), mMenuName(menuName),
			mIsAction(false)
	{
	}

	QAction *action() const {
		return mAction;
	}
	
	QMenu *menu() const {
		return mMenu;
	}

	QString toolbarName() const {
		return mToolbarName;
	}

	QString menuName() const {
		return mMenuName;
	}
	
	bool isAction() const {
		return mIsAction;
	}

private:
	QAction *mAction;
	QMenu *mMenu;
	
	QString mToolbarName;
	QString mMenuName;
	
	bool mIsAction;
};

}
