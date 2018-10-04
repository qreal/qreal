/* Copyright 2014-2016 QReal Research Group, Dmitry Chernov, Dmitry Mordvinov, CyberTech Labs Ltd.
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

#include "qrgui/mainWindow/scriptAPI/guiFacade.h"

#include <plugins/toolPluginInterface/toolPluginInterface.h>
#include <editor/editorView.h>
#include <qrutils/widgetFinder.h>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtTest/QTest>
#include <QtCore/QEvent>

#include "qrgui/mainWindow/mainWindow.h"
#include "qrgui/mainWindow/palette/draggableElement.h"
#include <qrgui/editor/propertyEditorView.h>
#include <qrgui/plugins/pluginManager/toolPluginManager.h>
using namespace qReal;
using namespace gui;

GuiFacade::GuiFacade(MainWindow &mainWindow)
	: mMainWindow(mainWindow)
{
}

QWidget *GuiFacade::widget(const QString &type, const QString &objectName) const
{
	return utils::WidgetFinder::widget(&mMainWindow, type, objectName);
}

QWidget *GuiFacade::findChildWidget(const QWidget *parent, const QString &type, const QString &objectName) const
{
	return utils::WidgetFinder::widget(parent, type, objectName);
}

DraggableElement *GuiFacade::draggableElement(const QString &widgetId) const
{
	QList<DraggableElement *> const paletteWidgets = mMainWindow.findChildren<DraggableElement *>();
	for (DraggableElement * const paletteElement : paletteWidgets) {
		if (paletteElement->id().toString() == widgetId) {
			return paletteElement;
		}
	}

	return nullptr;
}

QRect GuiFacade::propertyRect(const QString &name) const
{
	const QTreeWidget * const editorTree = mMainWindow.
			propertyEditor()->
			findChild<QtTreePropertyBrowser *>()->
			findChild<QTreeWidget *>();

	/// Adding 10 to coords needed to navigate in second column with interactive widgets.
	return QRect(editorTree->findChild<QWidget *>("qt_scrollarea_viewport")->mapTo(&mMainWindow
			, editorTree->visualItemRect(propertyTreeWidgetItem(name)).center() + QPoint(10, 0))
			, editorTree->visualItemRect(propertyTreeWidgetItem(name)).size());
}

QObject *GuiFacade::pluginGuiFacade(const QString &pluginName) const
{
	return mMainWindow.toolManager().pluginGuiFacade(pluginName);
}

QWidget *GuiFacade::property(const QString &name) const
{
	const QTreeWidget * const editorTree = mMainWindow.
			propertyEditor()->
			findChild<QtTreePropertyBrowser *>()->
			findChild<QTreeWidget *>();
	return editorTree->itemWidget(propertyTreeWidgetItem(name), 1);
}

QWidget *GuiFacade::pluginActionToolButton(const QString &name) const
{
	const QList<ActionInfo> actionList = mMainWindow.toolManager().actions();
	for (const ActionInfo &actionInfo : actionList) {
		if (actionInfo.action()->objectName() == name) {
			const QList<QWidget *> widgetList = actionInfo.action()->associatedWidgets();
			for (QWidget * const widget : widgetList) {
				const QString buttonClassName = "QToolButton";
				if (buttonClassName == widget->metaObject()->className() && widget->isVisible()) {
					return widget;
				}
			}
		}
	}

	return nullptr;
}

QWidget *GuiFacade::sceneViewport() const
{
	return mMainWindow.getCurrentTab()->viewport();
}

QWidget *GuiFacade::mainWindow() const
{
	return &mMainWindow;
}

QWidget *GuiFacade::propertyEditor() const
{
	const qReal::gui::editor::PropertyEditorView * const propertyEditor = mMainWindow.propertyEditor();
	return propertyEditor->
			findChild<QtTreePropertyBrowser *>()->
			findChild<QTreeWidget *>()->viewport();
}

QTreeWidgetItem *GuiFacade::propertyTreeWidgetItem(const QString &name) const
{
	const QTreeWidget * const editorTree = mMainWindow.
			propertyEditor()->
			findChild<QtTreePropertyBrowser *>()->
			findChild<QTreeWidget *>();

	for (int i = 0; i < editorTree->topLevelItemCount(); ++i) {
		QTreeWidgetItem * const item = editorTree->topLevelItem(i);
		if (item->data(0, Qt::DisplayRole).toString() == name) {
			return item;
		}
	}

	return nullptr;
}

QWidget *GuiFacade::deepViewPort(const QObject *object) const
{
	return object->findChild<QWidget *>("qt_scrollarea_viewport");
}

QWidget *GuiFacade::viewPort(const QAbstractScrollArea *widget) const
{
	return widget->viewport();
}

QMenu *GuiFacade::findMenu(const QString &menuName) const
{
	const QMenuBar *menuBar = mMainWindow.findChild<QMenuBar *>();
	QMenu * const menu = menuBar->findChild<QMenu *>(menuName, Qt::FindDirectChildrenOnly);
	return menu;
}

QAction *GuiFacade::findActionInMenu(const QMenu *menu, const QString &actionName) const
{
	const QList<QAction *> actions = menu->actions();
	for (QAction * const action : actions) {
		if (action->objectName() == actionName || action->text() == actionName) {
			return action;
		}
	}

	return nullptr;
}

bool GuiFacade::isSubMenuInMenu(const QMenu *menu, const QAction *action) const
{
	return menu->children().contains(action->menu());
}

QMenu *GuiFacade::findMenuContainedByAction(QAction *action) const
{
	emit action->menu()->aboutToShow(); // this is a workaround of the bug in qt (see guiFacade.h)
	return action->menu();
}

QWidget *GuiFacade::findStartButton(const QString &buttonText) const
{
	const QList<QPushButton *> allButtons = mMainWindow.findChildren<QPushButton *>();
	for (QPushButton * const button : allButtons) {
		if (button->isVisible() && button->objectName() == buttonText) {
			return button;
		}
	}

	return nullptr;
}
