#include <QtCore/QUuid>
#include <QtWidgets/QMessageBox>

#include "edgePropertiesDialog.h"
#include "ui_edgePropertiesDialog.h"
#include "../mainwindow/mainWindow.h"

using namespace qReal;

EdgePropertiesDialog::EdgePropertiesDialog(MainWindow *mainWindow, Id const &diagram, EditorManagerInterface const *editorManagerProxy, QWidget *parent) :
	QDialog(parent),
	mUi(new Ui::EdgePropertiesDialog),
	mMainWindow(mainWindow),
	mDiagram(diagram),
	mEditorManagerProxy(editorManagerProxy)
{
	mUi->setupUi(this);
	connect(mUi->okPushButton, SIGNAL(clicked()), this, SLOT(ok()));
}

EdgePropertiesDialog::~EdgePropertiesDialog()
{
	delete mUi;
}

void EdgePropertiesDialog::ok()
{
	if (mUi->nameEdit->text().isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("All required properties should be filled!"));
	} else {
		mEditorManagerProxy->addEdgeElement(mDiagram, mUi->nameEdit->text(), mUi->labelTextEdit->text(), mUi->labelTypeComboBox->currentText(),
								mUi->lineTypeComboBox->currentText(), mUi->beginTypeComboBox->currentText(), mUi->endTypeComboBox->currentText());
		mMainWindow->loadPlugins();
		done(1);
	}
}
