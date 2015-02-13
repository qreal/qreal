#include "chooseTypeDialog.h"
#include "ui_chooseTypeDialog.h"

#include <QtWidgets/QPushButton>

#include "dialogs/metamodelingOnFly/addNodeDialog.h"
#include "dialogs/metamodelingOnFly/edgePropertiesDialog.h"

using namespace qReal;
using namespace gui;

ChooseTypeDialog::ChooseTypeDialog(const Id &diagram
		, const EditorManagerInterface &editorManagerProxy
		, QWidget *parent)
		: QDialog(parent)
		, mUi(new Ui::ChooseTypeDialog)
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
		AddNodeDialog *nodeDialog = new AddNodeDialog(mDiagram, mEditorManagerProxy, parentWidget());
		connect(nodeDialog, &AddNodeDialog::jobDone, this, &ChooseTypeDialog::jobDone);
		nodeDialog->setModal(true);
		nodeDialog->show();
	} else if (mUi->edgeRadioButton->isChecked()) {
		EdgePropertiesDialog *edgeDialog = new EdgePropertiesDialog(mDiagram, mEditorManagerProxy, parentWidget());
		connect(edgeDialog, &EdgePropertiesDialog::jobDone, this, &ChooseTypeDialog::jobDone);
		edgeDialog->setModal(true);
		edgeDialog->show();
	}
}
