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

#include <QtGui/QStandardItemModel>

#include "restoreElementDialog.h"
#include "ui_restoreElementDialog.h"

using namespace qReal;

RestoreElementDialog::RestoreElementDialog(QWidget *parent
		, const EditorManagerInterface &interpreterEditorManager
		, const IdList &elementsWithTheSameNameList)
	: QDialog(parent)
	, mUi(new Ui::RestoreElementDialog)
	, mInterpreterEditorManager(interpreterEditorManager)
	, mElementsWithTheSameNameList(elementsWithTheSameNameList)
{
	mUi->setupUi(this);
	fillSameNameElementsTreeView();
	connect(mUi->restoreButton, &QPushButton::clicked, this, &RestoreElementDialog::restoreButtonClicked);
	connect(mUi->createNewButton, &QPushButton::clicked, this, &RestoreElementDialog::createButtonClicked);
}

RestoreElementDialog::~RestoreElementDialog()
{
	delete mUi;
}

void RestoreElementDialog::fillSameNameElementsTreeView()
{
	QStandardItemModel *standardModel = new QStandardItemModel();

	QStandardItem *item = standardModel->invisibleRootItem();
	for (const auto &element: mElementsWithTheSameNameList) {
		QString state = tr("Existed");
		if (mInterpreterEditorManager.getIsHidden(element) == "true") {
			state = tr("Deleted");
		}

		QList<QStandardItem *> elementRow = prepareRow(mInterpreterEditorManager.friendlyName(element)
				+ " (" + state + ")", "", "");
		for (QStandardItem *item: elementRow) {
			item->setEditable(false);
		}

		item->appendRow(elementRow);

		QStringList propertiesInformationList = mInterpreterEditorManager.getPropertiesInformation(element);
		for (int i = 0; i <= propertiesInformationList.count() - 3; i += 3) {
			QList<QStandardItem *> preparedRow = prepareRow(propertiesInformationList[i]
					, propertiesInformationList[i + 1], propertiesInformationList[i + 2]);
			for (QStandardItem *item: preparedRow) {
				item->setSelectable(false);
				item->setEditable(false);
			}

			elementRow.first()->appendRow(preparedRow);
		}
	}

	standardModel->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Type") << tr("Value"));
	mUi->sameNameElementsTreeView->setModel(standardModel);
	mUi->sameNameElementsTreeView->expandAll();
}

QList<QStandardItem *> RestoreElementDialog::prepareRow(const QString &first, const QString &second
		, const QString &third)
{
	 QList<QStandardItem *> rowItems;
	 rowItems << new QStandardItem(first);
	 rowItems << new QStandardItem(second);
	 rowItems << new QStandardItem(third);
	 return rowItems;
}

void RestoreElementDialog::restoreButtonClicked()
{
	if (mUi->sameNameElementsTreeView->selectionModel()->selectedIndexes().isEmpty()) {
		return;
	}

	const int selectedRow = mUi->sameNameElementsTreeView->selectionModel()->selectedIndexes().first().row();
	const Id node = mElementsWithTheSameNameList[selectedRow];
	if (mInterpreterEditorManager.getIsHidden(node) == "true") {
		mInterpreterEditorManager.resetIsHidden(node);
		emit jobDone();
	}

	emit restoreChosen(1);
	done(QDialog::Accepted);
}

void RestoreElementDialog::createButtonClicked()
{
	emit createNewChosen();
	done(QDialog::Accepted);
}
