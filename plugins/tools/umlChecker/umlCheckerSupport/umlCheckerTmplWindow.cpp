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

#include "umlCheckerTmplWindow.h"
#include "ui_umlCheckerTmplWindow.h"
#include <QtCore/QDir>

using namespace qReal;

UmlCheckerTmplWindow::UmlCheckerTmplWindow(QWidget *parent)
		: QDialog(parent)
		, mUi(new Ui::umlCheckerForm)
{
	mUi->setupUi(this);
	mUi->saveButton->setEnabled(true);
	mUi->saveEdgesButton->setEnabled(true);

	connect(mUi->saveButton, SIGNAL(clicked()), this, SLOT(blocksButtonActivate()));
	connect(mUi->saveEdgesButton, SIGNAL(clicked()), this, SLOT(edgesButtonActivate()));
	connect(mUi->addToCollection, SIGNAL(clicked()), this, SLOT(addToCollection()));
	connect(mUi->blockList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(openPicture(QListWidgetItem*)));

}

UmlCheckerTmplWindow::~UmlCheckerTmplWindow()
{

}

void UmlCheckerTmplWindow::openPicture(QListWidgetItem *item)
{
	const QString fileName = item->data(Qt::UserRole).toString();
	mUi->label->setScaledContents(true);
	mUi->label->setPixmap(QPixmap(fileName));
}

void UmlCheckerTmplWindow::setBlockName(const QString &blockName)
{
	mBlockName = blockName;
}

void UmlCheckerTmplWindow::openTemplatesForBlocks(const QString &fileName)
{
	QDir currentDir(fileName);
	QStringList filters;
	filters << "*.png";
	const QStringList pngFiles = currentDir.entryList(filters);
	QListWidget *blockList = mUi->blockList;
	blockList->clear();
	for (QString png : pngFiles) {
		png.chop(4);
		QListWidgetItem *item = new QListWidgetItem(png);
		item->setData(Qt::UserRole, fileName + png + ".png");
		item->setSelected(true);
		blockList->addItem(item);
	}

	if (blockList->count() > 0) {
		QListWidgetItem *firstItem = blockList->item(0);
		blockList->setCurrentItem(firstItem);
		openPicture(firstItem);
	}

}

QPair<QString, QStringList> UmlCheckerTmplWindow::getElementForBlock()
{
	QPair<QString, QStringList> result = qMakePair(mBlockName, mFiles);
	mFiles.clear();
	mBlockName = "";
	return result;
}

void UmlCheckerTmplWindow::addToCollection()
{
	QList<QListWidgetItem*> items = mUi->blockList->selectedItems();
	auto item = items.at(0);
	const QString fileName = item->data(Qt::UserRole).toString();
	mFiles.append(fileName);
}

void UmlCheckerTmplWindow::blocksButtonActivate()
{
	emit blocksButtonClicked();
}

void UmlCheckerTmplWindow::edgesButtonActivate()
{
	emit edgesButtonClicked();
}
