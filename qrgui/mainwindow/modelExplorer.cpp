#include "modelExplorer.h"
#include "../controller/commands/renameCommand.h"

using namespace qReal;
using namespace gui;
using namespace models::details::modelsImplementation;

ModelExplorer::ModelExplorer(QWidget *parent)
	: QTreeView(parent)
	, mController(NULL)
	, mModel(NULL)
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

void ModelExplorer::commitData(QWidget *editor)
{
	Id const id = static_cast<AbstractModel *>(model())->idByIndex(currentIndex());
	QString const oldName = model()->data(currentIndex()).toString();
	QTreeView::commitData(editor);
	QString const newName = model()->data(currentIndex()).toString();
	mController->execute(new commands::RenameCommand(mModel, id, oldName, newName));
}
