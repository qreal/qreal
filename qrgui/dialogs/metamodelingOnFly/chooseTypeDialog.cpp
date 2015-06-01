/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
