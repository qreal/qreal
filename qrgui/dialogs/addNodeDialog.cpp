#include "addNodeDialog.h"
#include "ui_addNodeDialog.h"
#include "../mainwindow/mainWindow.h"

using namespace qReal;

AddNodeDialog::AddNodeDialog(MainWindow *mainWindow, Id const &diagram, EditorManagerInterface const *editorManagerProxy, QWidget *parent) :
	QDialog(parent),
	mUi(new Ui::AddNodeDialog),
	mMainWindow(mainWindow),
	mDiagram(diagram),
	mEditorManagerProxy(editorManagerProxy)
{
	mUi->setupUi(this);
	connect(mUi->okPushButton, SIGNAL(clicked()), this, SLOT(ok()));
}

AddNodeDialog::~AddNodeDialog()
{
	delete mUi;
}

void AddNodeDialog::ok()
{
	if (mUi->nameEdit->text().isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("All required properties should be filled!"));
	} else {
		mEditorManagerProxy->addNodeElement(mDiagram, mUi->nameEdit->text(), mUi->checkBox->isChecked());
		mMainWindow->loadPlugins();
		done(1);
	}
}
