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

#include "renameDialog.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QDialogButtonBox>

using namespace qReal::gui;

RenameDialog::RenameDialog(const QString &initialText, QWidget *parent)
	: QDialog(parent), mInitialName(initialText)
{
	setWindowTitle(QCoreApplication::translate("qReal::gui::RenameDialog", "Enter new name"));

	mTextBox = new QLineEdit(mInitialName);
	mTextBox->setPlaceholderText(mInitialName);

	QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(mTextBox);
	layout->addWidget(buttons, 0, Qt::AlignRight);
	setLayout(layout);
}

RenameDialog::~RenameDialog()
{
}

QString RenameDialog::name() const
{
	return mTextBox->text();
}

QString RenameDialog::selectNewName(const QString &currentName, QWidget *parent)
{
	RenameDialog *dialog = new RenameDialog(currentName, parent);
	dialog->exec();
	const QString result = dialog->name();
	if (!parent) {
		delete dialog;
	}
	return result;
}

void RenameDialog::accept()
{
	if (mTextBox->text().isEmpty()) {
		mTextBox->setText(mInitialName);
	}
	QDialog::reject();
}

void RenameDialog::reject()
{
	mTextBox->setText(mInitialName);
	QDialog::reject();
}
