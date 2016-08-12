/* Copyright 2007-2016 QReal Research Group
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

#include <qrgui/controller/controller.h>
#include <qrgui/models/clipboard.h>
#include <qrgui/models/models.h>
#include <qrgui/models/commands/renameCommand.h>
#include <qrgui/models/details/modelsImplementation/abstractModel.h>

using namespace qReal;
using namespace gui;
using namespace models::details::modelsImplementation;

ModelExplorer::ModelExplorer(QWidget *parent)
	: QTreeView(parent)
	, mController(nullptr)
	, mModel(nullptr)
	, mExploser(nullptr)
	, mClipboard(nullptr)
	, mDeleteAction(tr("Delete"), this)
{
	mDeleteAction.setShortcut(QKeySequence(Qt::Key_Delete));
	connect(&mDeleteAction, &QAction::triggered, this, &ModelExplorer::elementRemoved);
	mDeleteAction.setEnabled(false);

	QAction * const deleteActionSeparator = new QAction(this);
	deleteActionSeparator->setSeparator(true);

	addAction(&mDeleteAction);
	addAction(deleteActionSeparator);
}

ModelExplorer::~ModelExplorer()
{
}

void ModelExplorer::initialize(Controller &controller, const models::Models &models
		, details::ModelsAssistInterface &model)
{
	mController = &controller;
	mModel = &model;
	mExploser = &models.exploser();
	mClipboard.reset(new models::Clipboard(controller, models));

	// This is bad, but for now we consider that registerEditor() was already invoked by system.
	// Later when model explorers will be pluginized this will be done manually in init(), so this
	// consideration will live for a while.
	addAction(mCopyAction);
	addAction(mPasteAction);
	addAction(mCutAction);
}

QString ModelExplorer::editorId() const
{
	return "qReal." + objectName();
}

bool ModelExplorer::supportsCopying() const
{
	return true;
}

bool ModelExplorer::supportsPasting() const
{
	return true;
}

bool ModelExplorer::supportsCutting() const
{
	return true;
}

void ModelExplorer::copy()
{
	const Id id = mModel->idByIndex(currentIndex());
	mClipboard->copy({id});
}

void ModelExplorer::paste()
{
	const Id selectedId = mModel->idByIndex(currentIndex());
	const Id rootId = selectedId.isNull() ? Id::rootId() : selectedId;
	mClipboard->paste(rootId, QPointF(), false);
}

void ModelExplorer::cut()
{
	copy();
	emit elementRemoved();
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

void ModelExplorer::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	QTreeView::selectionChanged(selected, deselected);
	updateActions();
}

void ModelExplorer::focusInEvent(QFocusEvent *event)
{
	QTreeView::focusInEvent(event);
	onFocusIn();
	updateActions();
	mDeleteAction.setEnabled(true);
}

void ModelExplorer::focusOutEvent(QFocusEvent *event)
{
	QTreeView::focusOutEvent(event);
	if (event->reason() != Qt::PopupFocusReason) {
		mDeleteAction.setEnabled(false);
	}
}

void ModelExplorer::updateActions()
{
	const bool itemSelected = !selectedIndexes().isEmpty();
	mCopyAction->setEnabled(itemSelected);
	mCutAction->setEnabled(itemSelected);
	mPasteAction->setEnabled(!mClipboard.isNull() && !mClipboard->isEmpty());
}
