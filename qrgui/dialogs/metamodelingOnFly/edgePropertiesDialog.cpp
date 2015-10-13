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

#include <QtCore/QUuid>
#include <QtWidgets/QMessageBox>

#include "edgePropertiesDialog.h"
#include "ui_edgePropertiesDialog.h"
#include "restoreElementDialog.h"

using namespace qReal;
using namespace gui;

EdgePropertiesDialog::EdgePropertiesDialog(const Id &diagram
		, const EditorManagerInterface &editorManagerProxy
		, QWidget *parent)
		: QDialog(parent)
		, mUi(new Ui::EdgePropertiesDialog)
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
		const IdList edgesWithTheSameNameList = mEditorManagerProxy.elementsWithTheSameName(mDiagram
				, mUi->nameEdit->text(), "MetaEntityEdge");
		if (!edgesWithTheSameNameList.isEmpty()) {
			mEdgeName = mUi->nameEdit->text() + "_" + edgesWithTheSameNameList.count();
			mRestoreElementDialog = new RestoreElementDialog(this, mEditorManagerProxy, edgesWithTheSameNameList);
			mRestoreElementDialog->setModal(true);
			mRestoreElementDialog->show();
			connect(mRestoreElementDialog, &qReal::RestoreElementDialog::createNewChosen
					, this, &EdgePropertiesDialog::addEdgeElement);
			connect(mRestoreElementDialog, &qReal::RestoreElementDialog::restoreChosen
					, this, &EdgePropertiesDialog::done);
			connect(mRestoreElementDialog, &qReal::RestoreElementDialog::jobDone
					, this, &EdgePropertiesDialog::jobDone);
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

	emit jobDone();
	done(QDialog::Accepted);
}
