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

#include <QtCore/QScopedPointer>
#include <QtWidgets/QActionGroup>

namespace twoDModel {
namespace view {

/// A container class for the set of inner actions used by 2D model widget.
class ActionsBox : public QObject
{
	Q_OBJECT

public:
	explicit ActionsBox(QObject *parent = 0);
	~ActionsBox();

	/// Returns a reference to action that switches scene to dragging mode (mouse interaction will move scene).
	QAction &scrollHandModeAction();

	/// Returns a reference to action that switches scene to rubber band drag mode
	/// (mouse interaction will draw selection rectangle).
	QAction &multiSelectionModeAction();

	/// Returns a reference to action that prompts user to export current world model to some file.
	QAction &saveModelAction();

	/// Returns a reference to action that prompts user to import current world model from some file.
	QAction &loadModelAction();

	/// Returns a list of actions that can be shown in scene context menu.
	QList<QAction *> sceneContextMenuActions();

	/// Shows or hides saveModelAction() and loadModelAction().
	void setSaveLoadActionsVisible(bool visible);

	/// Sets or unsets shortcuts to saveModelAction() and loadModelAction().
	void setSaveLoadActionsShortcutsEnabled(bool enabled);

private:
	QScopedPointer<QAction> mScrollHandModeAction;
	QScopedPointer<QAction> mMultiSelectionModeAction;
	QActionGroup mSceneModeActions;
	QScopedPointer<QAction> mSeparator;

	QScopedPointer<QAction> mSaveWorldModelAction;
	QScopedPointer<QAction> mLoadWorldModelAction;
};

}
}
