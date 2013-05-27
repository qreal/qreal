#include "chooseTypeDialog.h"
#include "ui_chooseTypeDialog.h"
#include "addNodeDialog.h"
#include "edgePropertiesDialog.h"

using namespace qReal;
using namespace gui;

ChooseTypeDialog::ChooseTypeDialog(MainWindow *mainWindow, PaletteTree const *paletteTree, EditorManagerInterface const *editorManagerProxy, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::ChooseTypeDialog)
	, mMainWindow(mainWindow)
	, mPaletteTree(paletteTree)
	, mEditorManagerProxy(editorManagerProxy)
{
	mUi->setupUi(this);
	connect(mUi->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(okButtonClicked()));
}

ChooseTypeDialog::~ChooseTypeDialog()
{
	delete mUi;
}

void ChooseTypeDialog::okButtonClicked() {
	Id const diagram = mPaletteTree->currentEditor();
	if (mUi->nodeRadioButton->isChecked()) {
		AddNodeDialog *nodeDialog = new AddNodeDialog(mMainWindow, diagram, mEditorManagerProxy);
		nodeDialog->setModal(true);
		nodeDialog->show();
	} else if (mUi->edgeRadioButton->isChecked()) {
		EdgePropertiesDialog *edgeDialog = new EdgePropertiesDialog(mMainWindow, diagram, mEditorManagerProxy);
		edgeDialog->setModal(true);
		edgeDialog->show();
	}
}
