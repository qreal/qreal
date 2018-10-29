/* Copyright 2018 CyberTech Labs Ltd.
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

#include "subprogramsCollectionDialog.h"
#include "ui_subprogramsCollectionDialog.h"

#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QListWidget>

SubprogramsCollectionDialog::SubprogramsCollectionDialog(QMap<QString, bool> &values, QWidget *parent) :
	QDialog(parent)
	, mUi(new Ui::subprogramsCollectionDialog)
	, mSelectMode(true)
	, mValues(values)
{
	mUi->setupUi(this);

	setWindowTitle(tr("Subprograms collection manager"));
	updateValues();

	connect(mUi->listWidget, &QListWidget::itemChanged, this, &SubprogramsCollectionDialog::highlightItem);
	connect(mUi->selectAllButton, &QPushButton::clicked, [this]() {
		for (int i=0; i < mUi->listWidget->count(); ++i) {
			mUi->listWidget->item(i)->setCheckState(mSelectMode ? Qt::Checked : Qt::Unchecked);
		}

		mSelectMode = not mSelectMode;
		if (mSelectMode) {
			mUi->selectAllButton->setText(tr("Select All"));
		} else {
			mUi->selectAllButton->setText(tr("Deselect All"));
		}
	});
}

SubprogramsCollectionDialog::~SubprogramsCollectionDialog()
{
	delete mUi;
}

void SubprogramsCollectionDialog::updateValues()
{
	mUi->listWidget->clear();
	mUi->selectAllButton->hide();
	for (auto file : mValues.keys()) {
		mUi->listWidget->addItem(file);
		QListWidgetItem *item = mUi->listWidget->item(mUi->listWidget->count() - 1);
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(mValues[file] ? Qt::Checked : Qt::Unchecked);
		highlightItem(item);
	}

	if (mValues.size() > 0) {
		mUi->selectAllButton->show();
	}
}

void SubprogramsCollectionDialog::accept()
{
	for (int i=0; i < mUi->listWidget->count(); ++i) {
		QListWidgetItem const * const item = mUi->listWidget->item(i);
		mValues[item->text()] = item->checkState() == Qt::Checked;
	}

	QDialog::accept();
}

void SubprogramsCollectionDialog::highlightItem(QListWidgetItem *item)
{
	if(item->checkState() == Qt::Checked) {
		item->setBackgroundColor(QColor("#ADFF2F"));
	} else {
		item->setBackgroundColor(QColor("#ffffff"));
	}
}
