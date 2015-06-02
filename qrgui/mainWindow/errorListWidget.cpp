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

#include "errorListWidget.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtGui/QClipboard>

#include "mainWindow/mainWindow.h"

ErrorListWidget::ErrorListWidget(QWidget *parent)
{
	Q_UNUSED(parent);
	connect(this, &ErrorListWidget::itemDoubleClicked, this, &ErrorListWidget::highlightElement);
	initContextMenu();
}

void ErrorListWidget::highlightElement(QListWidgetItem* const item)
{
	const qReal::Id id = qReal::Id::loadFromString(item->data(Qt::ToolTipRole).toString());
	if (item->isSelected()) {
		mMainWindow->selectItemWithError(id);
	}
}

void ErrorListWidget::init(qReal::MainWindow* mainWindow)
{
	mMainWindow = mainWindow;
}

void ErrorListWidget::initContextMenu()
{
	mContextMenu = new QMenu(this);
	QAction *clearAction = mContextMenu->addAction(tr("Clear"));
	QAction *copyAction = mContextMenu->addAction(tr("Copy"));
	connect(clearAction, SIGNAL(triggered()), this, SIGNAL(clearRequested()));
	connect(copyAction, SIGNAL(triggered()), this, SLOT(copyCurrentItem()));

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
}

void ErrorListWidget::showContextMenu(const QPoint &pos)
{
	mContextMenu->exec(mapToGlobal(pos));
}

void ErrorListWidget::copyCurrentItem()
{
	QListWidgetItem *item = currentItem();
	QLabel *label = item ? dynamic_cast<QLabel *>(itemWidget(item)) : nullptr;
	if (label) {
		// Extracting a plain text
		QTextDocument document;
		document.setHtml(label->text());
		QApplication::clipboard()->setText(document.toPlainText());
	}
}
