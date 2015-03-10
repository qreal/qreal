#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>

namespace qReal {

class ActionInfo
{
public:
	ActionInfo(QAction * const action, const QString &toolbarName, const QString &menuName)
		: mAction(action), mMenu(nullptr), mToolbarName(toolbarName), mMenuName(menuName), mIsAction(true)
	{
	}

	ActionInfo(QMenu * const menu, const QString &menuName)
		: mAction(nullptr), mMenu(menu), mToolbarName(""), mMenuName(menuName), mIsAction(false)
	{
	}

	QAction *action() const
	{
		return mAction;
	}

	QMenu *menu() const
	{
		return mMenu;
	}

	QString toolbarName() const
	{
		return mToolbarName;
	}

	QString menuName() const
	{
		return mMenuName;
	}

	bool isAction() const
	{
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
