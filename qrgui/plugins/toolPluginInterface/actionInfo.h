/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
