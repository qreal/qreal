#pragma once

#include <QtCore/QObject>
#include <QtGui/QAction>

namespace qReal {

class ActionInfo {
public:
	ActionInfo(QAction * const action, QString const &toolbarName, QString const &menuName)
	: mAction(action), mToolbarName(toolbarName), mMenuName(menuName)
	{
	}

	QAction *action() const {
		return mAction;
	}

	QString toolbarName() const {
		return mToolbarName;
	}

	QString menuName() const {
		return mMenuName;
	}

private:
	QAction *mAction;
	QString mToolbarName;
	QString mMenuName;
};

}
