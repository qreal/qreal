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

#include "modelExplorer.h"

#include <QtGui/QFocusEvent>

#include "models/commands/renameCommand.h"
#include "models/details/modelsImplementation/abstractModel.h"

using namespace qReal;
using namespace gui;
using namespace models::details::modelsImplementation;

ModelExplorer::ModelExplorer(QWidget *parent)
	: QTreeView(parent)
	, mController(nullptr)
	, mModel(nullptr)
	, mDeleteAction(tr("Delete"), this)
	, mDeleteActionSeparator(this)
{
	mDeleteAction.setShortcut(QKeySequence(Qt::Key_Delete));
	connect(&mDeleteAction, &QAction::triggered, this, &ModelExplorer::elementRemoved);
	mDeleteActionSeparator.setSeparator(true);
	mDeleteAction.setEnabled(false);
}

void ModelExplorer::setController(Controller * const controller)
{
	mController = controller;
}

void ModelExplorer::setAssistApi(details::ModelsAssistInterface * const model)
{
	mModel = model;
}

void ModelExplorer::setExploser(const models::Exploser &exploser)
{
	mExploser = &exploser;
}

void ModelExplorer::changeEditorActionsSet(QList<QAction *> const &actions)
{
	for (QAction * const action : this->actions()) {
		removeAction(action);
	}

	addAction(&mDeleteAction);
	addAction(&mDeleteActionSeparator);
	addActions(actions);
}

void ModelExplorer::commitData(QWidget *editor)
{
	const Id id = static_cast<AbstractModel *>(model())->idByIndex(currentIndex());
	const QString oldName = model()->data(currentIndex()).toString();
	QTreeView::commitData(editor);
	const QString newName = model()->data(currentIndex()).toString();
	if (oldName != newName) {
		mController->execute(new commands::RenameCommand(*mModel, id, oldName, newName, mExploser));
	}
}


void ModelExplorer::focusOutEvent(QFocusEvent *event)
{
	if (event->reason() != Qt::PopupFocusReason) {
		setActionsEnabled(false);
	}
}

void ModelExplorer::focusInEvent(QFocusEvent *event)
{
	Q_UNUSED(event)
	setActionsEnabled(true);
}

void ModelExplorer::setActionsEnabled(bool enabled)
{
	for (QAction * const action : actions()) {
		action->setEnabled(enabled);
	}
}
