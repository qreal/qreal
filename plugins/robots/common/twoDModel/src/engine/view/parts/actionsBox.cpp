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

#include "actionsBox.h"

using namespace twoDModel::view;

ActionsBox::ActionsBox(QObject *parent)
	: QObject(parent)
	, mScrollHandModeAction(new QAction(QIcon(":/icons/2d_hand.png"), tr("Hand dragging mode"), nullptr))
	, mMultiSelectionModeAction(new QAction(QIcon(":/icons/2d_multiselection.png"), tr("Multiselection mode"), nullptr))
	, mSceneModeActions(this)
	, mSeparator(new QAction(nullptr))
	, mSaveWorldModelAction(new QAction(QIcon(":/icons/2d_save.png"), tr("Save world model..."), nullptr))
	, mLoadWorldModelAction(new QAction(QIcon(":/icons/2d_open.png"), tr("Load world model..."), nullptr))
{
	mScrollHandModeAction->setCheckable(true);
	mMultiSelectionModeAction->setCheckable(true);
	mSceneModeActions.addAction(mScrollHandModeAction.data());
	mSceneModeActions.addAction(mMultiSelectionModeAction.data());
	mSeparator->setSeparator(true);
}

ActionsBox::~ActionsBox()
{
}

QAction &ActionsBox::scrollHandModeAction()
{
	return *mScrollHandModeAction;
}

QAction &ActionsBox::multiSelectionModeAction()
{
	return *mMultiSelectionModeAction;
}

QAction &ActionsBox::saveModelAction()
{
	return *mSaveWorldModelAction;
}

QAction &ActionsBox::loadModelAction()
{
	return *mLoadWorldModelAction;
}

QList<QAction *> ActionsBox::sceneContextMenuActions()
{
	return {
		&scrollHandModeAction()
		, &multiSelectionModeAction()
		, mSeparator.data()
		, &saveModelAction()
		, &loadModelAction()
	};
}

void ActionsBox::setSaveLoadActionsVisible(bool visible)
{
	saveModelAction().setVisible(visible);
	loadModelAction().setVisible(visible);
}

void ActionsBox::setSaveLoadActionsShortcutsEnabled(bool enabled)
{
	if (enabled) {
		saveModelAction().setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
		loadModelAction().setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	} else {
		saveModelAction().setShortcut(QKeySequence());
		loadModelAction().setShortcut(QKeySequence());
	}
}
