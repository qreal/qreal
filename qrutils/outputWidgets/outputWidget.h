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

#include <QtWidgets/QWidget>

namespace utils {

/// Base class for widgets displaying something in a bottom dock. Can request its visibility change.
class OutputWidget : public QWidget
{
	Q_OBJECT
public:
	explicit OutputWidget(QWidget *parent = 0);

	/// Initializes real displaying widget.
	virtual void setWidget(QWidget *widget);

	/// Returns title for a corresponding tab.
	virtual QString title() const = 0;

	/// Returns action which would open/close a widget.
	virtual QAction *action() = 0;

	/// Returns a name of shortcut for registering in HotKeyManager.
	virtual QString shortcutName() const = 0;

signals:
	/// Emitted when a widget wants to change its visibility.
	void toggleVisibility();

	/// Emitted when a widget wants to display something.
	void showRequest();

	/// Emitted when a widget wants to close output dock.
	void hideRequest();
};

}
