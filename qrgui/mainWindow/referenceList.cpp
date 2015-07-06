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

#include "referenceList.h"
#include "ui_referenceList.h"

#include "mainWindow/mainWindow.h"

ReferenceList::ReferenceList(qReal::MainWindow *mainWindow, const QPersistentModelIndex &index
		, const QString &refType, const QStringList &currentValue, int role, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::ReferenceList)
	, mIndex(index)
	, mRole(role)
	, mWindow(mainWindow)
{
	mUi->setupUi(this);

	loadList(refType);
	highlightCurrentValue(currentValue);

	initConnections();
}

ReferenceList::~ReferenceList()
{
	delete mUi;
}

void ReferenceList::loadList(const QString &refType)
{
	const qrRepo::LogicalRepoApi *repoApi = &(mWindow->models().logicalRepoApi());
	// there will be no need in this when models are synchronized
	qReal::models::GraphicalModelAssistApi &assistApi = mWindow->models().graphicalModelAssistApi();

	foreach (qReal::Id element, repoApi->elementsByType(refType)) {
		if (repoApi->isLogicalElement(element) && !assistApi.graphicalIdsByLogicalId(element).isEmpty()) {
			addItem(element);
		}
	}
}

void ReferenceList::addItem(const qReal::Id &element)
{
	QString name = mWindow->models().logicalRepoApi().name(element);

	QListWidgetItem *item = new QListWidgetItem(name, mUi->listWidget);
	item->setData(Qt::UserRole, element.toString());
}

void ReferenceList::highlightCurrentValue(const QStringList &currentValue)
{
	for (int i = 0; i < mUi->listWidget->count(); i++) {
		QListWidgetItem* currItem = mUi->listWidget->item(i);
		if (currentValue.contains(currItem->data(Qt::UserRole).toString())) {
			currItem->setSelected(true);
		}
	}
}

void ReferenceList::initConnections()
{
	connect(mUi->listWidget, SIGNAL(itemClicked(QListWidgetItem*))
			, this, SLOT(activateElement(QListWidgetItem*)));
	connect(this, SIGNAL(accepted()), this, SLOT(valueChanged()));
	connect(this, SIGNAL(finished(int)), this, SLOT(restoreSelected()));
}

void ReferenceList::activateElement(QListWidgetItem *item)
{
	qReal::Id id = qReal::Id::loadFromString(item->data(Qt::UserRole).toString());
	mWindow->activateItemOrDiagram(id, true);
}

void ReferenceList::valueChanged()
{
	QStringList newValue = getNewValue();
	emit referenceSet(newValue, mIndex, mRole);
}

QStringList ReferenceList::getNewValue() const
{
	QStringList newValue;
	foreach(QListWidgetItem *item, mUi->listWidget->selectedItems()) {
		newValue << item->data(Qt::UserRole).toString();
	}
	return newValue;
}

void ReferenceList::restoreSelected()
{
	qReal::Id indexId = mWindow->models().logicalModelAssistApi().idByIndex(mIndex);
	mWindow->activateItemOrDiagram(indexId, true);
}
