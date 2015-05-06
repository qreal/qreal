/* Copyright 2015 QReal Research Group, Dmitry Mordvinov
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

class QToolBar;
class QAction;
class QActionGroup;

namespace twoDModel {
namespace view {

/// A widget containing a set of items that can be drawn on 2D model scene.
class Palette : public QWidget
{
	Q_OBJECT

public:
	explicit Palette(QWidget *parent = 0);
	~Palette();

	/// Returns a pointer to 'none' tool that triggered when no other tool selected.
	const QAction &cursorAction() const;

public slots:
	/// Appends an item to 2D model`s palette.
	void registerTool(QAction * const tool);

	/// Activates cursor action.
	void unselect();

private:
	QScopedPointer<QAction> mCursorAction;
	QScopedPointer<QActionGroup> mGroup;
	QToolBar *mToolBar;  // Takes ownership
};

}
}
