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

#include "addNodeDialog.h"
#include "ui_addNodeDialog.h"

#include <QtWidgets/QMessageBox>

#include "restoreElementDialog.h"

using namespace qReal;
using namespace gui;

AddNodeDialog::AddNodeDialog(const Id &diagram
		, const EditorManagerInterface &editorManagerProxy
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
		const IdList nodesWithTheSameNameList = mEditorManagerProxy.elementsWithTheSameName(mDiagram
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
