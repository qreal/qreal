/* Copyright 2007-2016 QReal Research Group, Dmitry Mordvinov
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

#include "tabWidget.h"

#include <QtGui/QMouseEvent>
#include <QtWidgets/QTabBar>

using namespace qReal::gui;

TabWidget::TabWidget(QWidget *parent)
	: QTabWidget(parent)
{
	connect(this, &QTabWidget::currentChanged, this, &TabWidget::onTabChanged);
}

void TabWidget::setTabUnclosable(int index)
{
	QWidget *tabButton = tabBar()->tabButton(index, QTabBar::RightSide);
	if (!tabButton) {
		tabButton = tabBar()->tabButton(index, QTabBar::LeftSide);
	}

	if (tabButton) {
		tabButton->resize(0, 0);
	}
}

QString TabWidget::editorId() const
{
	return currentEditor() ? currentEditor()->editorId() : "qReal.TabWidget";
}

bool TabWidget::supportsZooming() const
{
	return currentEditor() ? currentEditor()->supportsZooming() : false;
}

bool TabWidget::supportsCopying() const
{
	return currentEditor() ? currentEditor()->supportsCopying() : false;
}

bool TabWidget::supportsPasting() const
{
	return currentEditor() ? currentEditor()->supportsPasting() : false;
}

bool TabWidget::supportsCutting() const
{
	return currentEditor() ? currentEditor()->supportsCutting() : false;
}

bool TabWidget::supportsSearching() const
{
	return currentEditor() ? currentEditor()->supportsSearching() : false;
}

bool TabWidget::supportsFindAndReplace() const
{
	return currentEditor() ? currentEditor()->supportsFindAndReplace() : false;
}

bool TabWidget::supportsReplacingBy() const
{
	return currentEditor() ? currentEditor()->supportsReplacingBy() : false;
}

void TabWidget::zoomIn()
{
	if (EditorInterface *editor = currentEditor()) {
		editor->zoomIn();
	}
}

void TabWidget::zoomOut()
{
	if (EditorInterface *editor = currentEditor()) {
		editor->zoomOut();
	}
}

void TabWidget::copy()
{
	if (EditorInterface *editor = currentEditor()) {
		editor->copy();
	}
}

void TabWidget::paste()
{
	if (EditorInterface *editor = currentEditor()) {
		editor->paste();
	}
}

void TabWidget::cut()
{
	if (EditorInterface *editor = currentEditor()) {
		editor->cut();
	}
}

void TabWidget::find()
{
	if (EditorInterface *editor = currentEditor()) {
		editor->find();
	}
}

void TabWidget::findAndReplace()
{
	if (EditorInterface *editor = currentEditor()) {
		editor->findAndReplace();
	}
}

void TabWidget::replaceBy()
{
	if (EditorInterface *editor = currentEditor()) {
		editor->replaceBy();
	}
}

void TabWidget::forceFocus()
{
	onTabChanged();
}

int TabWidget::addTab(QWidget *widget, const QString &name)
{
	if (EditorInterface * const editor = dynamic_cast<EditorInterface *>(widget)) {
		editor->configure(*mZoomInAction, *mZoomOutAction, *mUndoAction, *mRedoAction
				, *mCopyAction, *mPasteAction, *mCutAction, *mFindAction, *mFindAndReplaceAction, *mReplaceByAction);
		connect(&editor->focusAction(), &QAction::triggered, &focusAction(), &QAction::trigger);
	}

	return QTabWidget::addTab(widget, name);
}

void TabWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::MiddleButton && tabBar()->tabAt(event->pos()) > -1) {
		removeTab(tabBar()->tabAt(event->pos()));
	}
}

qReal::EditorInterface *TabWidget::currentEditor() const
{
	return dynamic_cast<EditorInterface *>(currentWidget());
}

void TabWidget::onTabChanged()
{
	if (currentEditor()) {
		onFocusIn();
	}
}
