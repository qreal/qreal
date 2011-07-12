#pragma once

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtGui/QAction>

namespace qReal {

class CustomToolInterface
{
public:
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

	virtual QList<ActionInfo> actions() const = 0;
	virtual void init() = 0;
};

}
