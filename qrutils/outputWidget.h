/* Copyright 2015 QReal Research Group
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

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QAction>

namespace utils {

/// Base class for widgets displaying something in a bottom dock. Can request its visibility change.
class OutputWidget : public QDockWidget
{
	Q_OBJECT
public:
	explicit OutputWidget(const QString &title, QWidget *parent = 0);

	QAction *action();

	/// Returns a name of shortcut for registering in HotKeyManager.
	virtual QString shortcutName() const = 0;

signals:
	void showMe();

private:
	QAction mAction;
};

}
