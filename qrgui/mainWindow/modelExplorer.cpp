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

void ModelExplorer::setExploser(models::Exploser const &exploser)
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
