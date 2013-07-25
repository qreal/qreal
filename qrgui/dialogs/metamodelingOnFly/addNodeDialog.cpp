#include "addNodeDialog.h"

#include "ui_addNodeDialog.h"
#include "../../mainwindow/mainWindow.h"

using namespace qReal;
using namespace gui;

AddNodeDialog::AddNodeDialog(MainWindow &mainWindow, Id const &diagram, EditorManagerInterface const &editorManagerProxy)
		: QDialog(&mainWindow)
		, mUi(new Ui::AddNodeDialog)
		, mMainWindow(mainWindow)
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
		mEditorManagerProxy.addNodeElement(mDiagram, mUi->nameEdit->text(), mUi->checkBox->isChecked());
		mMainWindow.loadPlugins();
		done(QDialog::Accepted);
	}
}
