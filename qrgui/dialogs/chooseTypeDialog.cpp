#include "chooseTypeDialog.h"
#include "ui_chooseTypeDialog.h"
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
		bool ok;
		QString name = QInputDialog::getText(this, tr("Enter the element name:"), tr("Element name:"), QLineEdit::Normal, "", &ok);
		while (ok && name.isEmpty()) {
			name = QInputDialog::getText(this, tr("Enter the element name:"), tr("Element name:"), QLineEdit::Normal, "", &ok);
		}
		if (ok) {
			mEditorManagerProxy->addNodeElement(diagram, name);
			mMainWindow->loadPlugins();
		}
	} else {
		EdgePropertiesDialog *edgeDialog = new EdgePropertiesDialog(mMainWindow, diagram, mEditorManagerProxy);
		edgeDialog->setModal(true);
		edgeDialog->show();
	}
}
