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

#include "refactoringWindow.h"
#include "ui_refactoringWindow.h"
#include <QtCore/QDir>

using namespace qReal;

RefactoringWindow::RefactoringWindow(QWidget *parent)
		: QDialog(parent)
		, mUi(new Ui::refactoringForm)
{
	mUi->setupUi(this);
	mUi->applyButton->setEnabled(false);
	mUi->discardButton->setEnabled(false);
	mUi->findNextButton->setEnabled(false);

	connect(mUi->refactoringList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(openPicture(QListWidgetItem*)));
	connect(mUi->findButton, SIGNAL(clicked()), this, SLOT(findButtonActivate()));
	connect(mUi->findNextButton, SIGNAL(clicked()), this, SLOT(findNextButtonActivate()));
	connect(mUi->discardButton, SIGNAL(clicked()), this, SLOT(discardButtonActivate()));
	connect(mUi->applyButton, SIGNAL(clicked()), this, SLOT(applyButtonActivate()));
}

void RefactoringWindow::openPicture(QListWidgetItem *item)
{
	QString const fileName = item->data(Qt::UserRole).toString();
	mUi->label->setScaledContents(true);
	mUi->label->setPixmap(QPixmap(fileName));
}

RefactoringWindow::~RefactoringWindow()
{
	delete mUi;
}

void RefactoringWindow::updateRefactorings(const QString &dirPath)
{
	QDir currentDir(dirPath);
	QStringList filters;
	filters << "*.png";
	QStringList const pngFiles = currentDir.entryList(filters);
	QListWidget *refactoringList = mUi->refactoringList;
	refactoringList->clear();
	foreach (QString png, pngFiles) {
		png.chop(4);
		QListWidgetItem *item = new QListWidgetItem(png);
		item->setData(Qt::UserRole, dirPath + png + ".png");
		refactoringList->addItem(item);
	}

	if (refactoringList->count() > 0) {
		QListWidgetItem *firstItem = refactoringList->item(0);
		refactoringList->setCurrentItem(firstItem);
		openPicture(firstItem);
	}
}

void RefactoringWindow::findButtonActivate()
{
	QList<QListWidgetItem*> selectedItems = mUi->refactoringList->selectedItems();
	if (selectedItems.size() != 1) {
		return;
	}
	emit findButtonClicked(selectedItems.at(0)->text());
}

void RefactoringWindow::findNextButtonActivate()
{
	emit findNextButtonClicked();
}


void RefactoringWindow::activateRestButtons()
{
	mUi->applyButton->setEnabled(true);
	mUi->discardButton->setEnabled(true);
	mUi->findNextButton->setEnabled(true);
	mUi->findButton->setEnabled(false);
}

void RefactoringWindow::discard()
{
	mUi->applyButton->setEnabled(false);
	mUi->discardButton->setEnabled(false);
	mUi->findNextButton->setEnabled(false);
	mUi->findButton->setEnabled(true);

}

void RefactoringWindow::discardButtonActivate()
{
	emit discardButtonClicked();
}

void RefactoringWindow::applyButtonActivate()
{
	emit applyButtonClicked();
}
