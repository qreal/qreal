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
#include <QtWidgets/QMenu>
#include <QtGui/QClipboard>
#include <QtGui/QTextDocument>

#include <qrkernel/exception/exception.h>

using namespace qReal::gui;

ErrorListWidget::ErrorListWidget(QWidget *parent)
	: OutputWidget(tr("Issues"), parent)
{
	setWidget(&mListWidget);
	setObjectName("errorReporter");

	connect(&mListWidget, &QListWidget::itemDoubleClicked, this, &ErrorListWidget::highlightElement);
	initContextMenu();

	action()->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
}

void ErrorListWidget::highlightElement(QListWidgetItem* const item)
{
	const qReal::Id id = qReal::Id::loadFromString(item->data(Qt::ToolTipRole).toString());
	if (item->isSelected()) {
		emit highlightId(id);
	}
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
	QListWidgetItem *item = mListWidget.currentItem();
	QLabel *label = item ? dynamic_cast<QLabel *>(mListWidget.itemWidget(item)) : nullptr;
	if (label) {
		// Extracting a plain text
		QTextDocument document;
		document.setHtml(label->text());
		QApplication::clipboard()->setText(document.toPlainText());
	}
}

int ErrorListWidget::count() const
{
	return mListWidget.count();
}

void ErrorListWidget::clear()
{
	mListWidget.clear();
	hide();
}

void ErrorListWidget::addError(const Error &error)
{
	QListWidgetItem *item = new QListWidgetItem();
	item->setIcon(errorIcon(error));
	item->setToolTip(error.position().toString());

	const QString message = QString(" <font color='gray'>%1</font> <u>%2</u> %3").arg(
			error.timestamp(), severityMessage(error), error.message());
	QLabel *label = new QLabel(message.trimmed());
	label->setAlignment(Qt::AlignVCenter);
	label->setOpenExternalLinks(true);

	mListWidget.addItem(item);
	mListWidget.setItemWidget(item, label);
	mListWidget.setCurrentItem(item);

	emit showMe();
}

QString ErrorListWidget::severityMessage(const Error &error)
{
	switch (error.severity()) {
	case Error::information:
		return tr("INFORMATION:");
	case Error::warning:
		return tr("WARNING:");
	case Error::error:
		return tr("ERROR:");
	case Error::critical:
		return tr("CRITICAL:");
	default:
		throw new Exception("Incorrect severity of an error");
	}
}

QIcon ErrorListWidget::errorIcon(const Error &error)
{
	switch (error.severity()) {
	case Error::information:
		return QIcon(":/mainWindow/images/information.png");
	case Error::warning:
		return QIcon(":/mainWindow/images/warning.png");
	case Error::error:
		return QIcon(":/mainWindow/images/error.png");
	case Error::critical:
		return QIcon(":/mainWindow/images/critical.png");
	default:
		throw new Exception("Incorrect total severity");
	}
}

QString ErrorListWidget::shortcutName() const
{
	return "Output.Errors";
}
