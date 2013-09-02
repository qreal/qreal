#include "chooseTypeDialog.h"
#include "ui_chooseTypeDialog.h"

#include "dialogs/metamodelingOnFly/addNodeDialog.h"
#include "dialogs/metamodelingOnFly/edgePropertiesDialog.h"

using namespace qReal;
using namespace gui;

ChooseTypeDialog::ChooseTypeDialog(MainWindow &mainWindow, Id const &diagram
		, EditorManagerInterface const &editorManagerProxy)
		: QDialog(&mainWindow)
		, mUi(new Ui::ChooseTypeDialog)
		, mMainWindow(mainWindow)
		, mDiagram(diagram)
		, mEditorManagerProxy(editorManagerProxy)
{
	mUi->setupUi(this);
	connect(mUi->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(okButtonClicked()));
}

ChooseTypeDialog::~ChooseTypeDialog()
{
	delete mUi;
}

void ChooseTypeDialog::okButtonClicked()
{
	if (mUi->nodeRadioButton->isChecked()) {
		AddNodeDialog *nodeDialog = new AddNodeDialog(mMainWindow, mDiagram, mEditorManagerProxy);
		nodeDialog->setModal(true);
		nodeDialog->show();
	} else if (mUi->edgeRadioButton->isChecked()) {
		EdgePropertiesDialog *edgeDialog = new EdgePropertiesDialog(mMainWindow, mDiagram, mEditorManagerProxy);
		edgeDialog->setModal(true);
		edgeDialog->show();
	}
}
