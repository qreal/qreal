#include "modelExplorer.h"

#include <QtWidgets/QAction>
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
{
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

void ModelExplorer::changeActionsSet(QList<QAction *> const &actions)
{
	for (QAction *action : this->actions()) {
		removeAction(action);
	}

	addActions(actions);
}

void ModelExplorer::commitData(QWidget *editor)
{
	Id const id = static_cast<AbstractModel *>(model())->idByIndex(currentIndex());
	QString const oldName = model()->data(currentIndex()).toString();
	QTreeView::commitData(editor);
	QString const newName = model()->data(currentIndex()).toString();
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
	for (QAction *action : actions()) {
		action->setEnabled(enabled);
	}
}
