#include "addNodeDialog.h"
#include "ui_addNodeDialog.h"

#include <QtWidgets/QMessageBox>

#include "restoreElementDialog.h"

using namespace qReal;
using namespace gui;

AddNodeDialog::AddNodeDialog(Id const &diagram
		, EditorManagerInterface const &editorManagerProxy
		, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::AddNodeDialog)
	, mDiagram(diagram)
	, mEditorManagerProxy(editorManagerProxy)
{
	mUi->setupUi(this);
	connect(mUi->okPushButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
}

AddNodeDialog::~AddNodeDialog()
{
	delete mUi;
}

void AddNodeDialog::okButtonClicked()
{
	if (mUi->nameEdit->text().isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("All required properties should be filled!"));
	} else {
		mNodeName = mUi->nameEdit->text();
		IdList const nodesWithTheSameNameList = mEditorManagerProxy.elementsWithTheSameName(mDiagram
				, mUi->nameEdit->text(), "MetaEntityNode");
		if (!nodesWithTheSameNameList.isEmpty()) {
			mNodeName = mUi->nameEdit->text() + "_" + nodesWithTheSameNameList.count();
			mRestoreElementDialog = new RestoreElementDialog(this, mEditorManagerProxy, nodesWithTheSameNameList);
			mRestoreElementDialog->setModal(true);
			mRestoreElementDialog->show();
			connect(mRestoreElementDialog, &qReal::RestoreElementDialog::createNewChosen
					, this, &AddNodeDialog::addNode);
			connect(mRestoreElementDialog, &qReal::RestoreElementDialog::restoreChosen
					, this, &AddNodeDialog::done);
			connect(mRestoreElementDialog, &qReal::RestoreElementDialog::jobDone
					, this, &AddNodeDialog::jobDone);
		} else {
			addNode();
		}
	}
}

void AddNodeDialog::addNode()
{
	mEditorManagerProxy.addNodeElement(mDiagram, mNodeName, mUi->nameEdit->text(), mUi->checkBox->isChecked());
	emit jobDone();
	done(QDialog::Accepted);
}
