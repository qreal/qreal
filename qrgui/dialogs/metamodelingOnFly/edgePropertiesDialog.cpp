#include <QtCore/QUuid>
#include <QtWidgets/QMessageBox>

#include "edgePropertiesDialog.h"
#include "ui_edgePropertiesDialog.h"
#include "restoreElementDialog.h"

#include "mainwindow/mainWindow.h"

using namespace qReal;
using namespace gui;

EdgePropertiesDialog::EdgePropertiesDialog(MainWindow &mainWindow, Id const &diagram
		, EditorManagerInterface const &editorManagerProxy)
		: QDialog(&mainWindow)
		, mUi(new Ui::EdgePropertiesDialog)
		, mMainWindow(mainWindow)
		, mDiagram(diagram)
		, mEditorManagerProxy(editorManagerProxy)
{
	mUi->setupUi(this);
	connect(mUi->okPushButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
}

EdgePropertiesDialog::~EdgePropertiesDialog()
{
	delete mUi;
}

void EdgePropertiesDialog::okButtonClicked()
{
	if (mUi->nameEdit->text().isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("All required properties should be filled!"));
	} else {
		mEdgeName = mUi->nameEdit->text();
		IdList const edgesWithTheSameNameList = mEditorManagerProxy.elementsWithTheSameName(mDiagram
				, mUi->nameEdit->text(), "MetaEntityEdge");
		if (!edgesWithTheSameNameList.isEmpty()) {
			mEdgeName = mUi->nameEdit->text() + "_" + edgesWithTheSameNameList.count();
			mRestoreElementDialog = new RestoreElementDialog(this, mMainWindow, mEditorManagerProxy, edgesWithTheSameNameList);
			mRestoreElementDialog->setModal(true);
			mRestoreElementDialog->show();
			connect(mRestoreElementDialog, &qReal::RestoreElementDialog::createNewChosen
					, this, &EdgePropertiesDialog::addEdgeElement);
			connect(mRestoreElementDialog, &qReal::RestoreElementDialog::restoreChosen
					, this, &EdgePropertiesDialog::done);
		} else {
			addEdgeElement();
		}
	}
}

void EdgePropertiesDialog::addEdgeElement()
{
	mEditorManagerProxy.addEdgeElement(
			mDiagram
			, mEdgeName
			, mUi->nameEdit->text()
			, mUi->labelTextEdit->text()
			, mUi->labelTypeComboBox->currentText()
			, mUi->lineTypeComboBox->currentText()
			, mUi->beginTypeComboBox->currentText()
			, mUi->endTypeComboBox->currentText()
			);

	mMainWindow.loadPlugins();
	done(QDialog::Accepted);
}
