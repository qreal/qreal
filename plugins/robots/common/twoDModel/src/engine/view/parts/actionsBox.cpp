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
	, mSceneModeActions(this)
	, mScrollHandModeAction(new QAction(QIcon(":/icons/2d_hand.png"), tr("Hand dragging mode"), nullptr))
	, mMultiSelectionModeAction(new QAction(QIcon(":/icons/2d_multiselection.png"), tr("Multiselection mode"), nullptr))
	, mSeparator1(new QAction(nullptr))
	, mSaveWorldModelAction(new QAction(QIcon(":/icons/2d_save.png"), tr("Save world model..."), nullptr))
	, mLoadWorldModelAction(new QAction(QIcon(":/icons/2d_open.png"), tr("Load world model..."), nullptr))
	, mSeparator2(new QAction(nullptr))
	, mClearFloorAction(new QAction(QIcon(":/icons/2d_clear_floor.svg"), tr("Clear floor"), nullptr))
{
	mScrollHandModeAction->setCheckable(true);
	mMultiSelectionModeAction->setCheckable(true);
	mSceneModeActions.addAction(mScrollHandModeAction.data());
	mSceneModeActions.addAction(mMultiSelectionModeAction.data());
	mSeparator1->setSeparator(true);
	mSeparator2->setSeparator(true);
	connect(mClearFloorAction.data(), &QAction::changed
			, [=]() { mSeparator2->setVisible(mClearFloorAction->isVisible()); });
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

QAction &ActionsBox::clearFloorAction()
{
	return *mClearFloorAction;
}

QList<QAction *> ActionsBox::sceneContextMenuActions()
{
	return {
		&scrollHandModeAction()
		, &multiSelectionModeAction()
		, mSeparator1.data()
		, &saveModelAction()
		, &loadModelAction()
		, mSeparator2.data()
		, &clearFloorAction()
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
