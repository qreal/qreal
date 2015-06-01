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

#include "listWidget.h"

#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

using namespace qReal;

ListWidget::ListWidget(QWidget *parent)
	: QWidget(parent)
	, mListWidget(new QListWidget())
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(mListWidget);
	setLayout(mainLayout);

	connect(mListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*))
			, this, SLOT(doubleClickHandler(QListWidgetItem*)));
}

void ListWidget::addItem(const QString &text, const QString &userData, const QString &toolTip)
{
	QListWidgetItem *currentItem = new QListWidgetItem(text, mListWidget);
	currentItem->setData(Qt::UserRole, userData);
	currentItem->setToolTip(toolTip);
	mListWidget->addItem(currentItem);
}

void ListWidget::highlightFirstItem()
{
	if (mListWidget->count()) {
		mListWidget->setCurrentRow(0);
	}
}

void ListWidget::doubleClickHandler(QListWidgetItem *item)
{
	emit userDataSelected(userData(item));
}

QString ListWidget::userData(QListWidgetItem *item)
{
	return item->data(Qt::UserRole).toString();
}
