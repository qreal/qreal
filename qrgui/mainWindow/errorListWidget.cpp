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
	qReal::Id const id = qReal::Id::loadFromString(item->data(Qt::ToolTipRole).toString());
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

void ErrorListWidget::showContextMenu(QPoint const &pos)
{
	mContextMenu->exec(mapToGlobal(pos));
}

void ErrorListWidget::copyCurrentItem()
{
	QListWidgetItem *item = currentItem();
	QLabel *label = item ? dynamic_cast<QLabel *>(itemWidget(item)) : NULL;
	if (label) {
		// Extracting a plain text
		QTextDocument document;
		document.setHtml(label->text());
		QApplication::clipboard()->setText(document.toPlainText());
	}
}
