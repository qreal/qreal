/* Copyright 2014-2015 QReal Research Group, Dmitry Chernov, Dmitry Mordvinov
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

#include "./qrgui/mainWindow/scriptAPI/guiFacade.h"

#include <plugins/toolPluginInterface/toolPluginInterface.h>
#include <editor/editorView.h>
#include <qrutils/widgetFinder.h>
#include <QMenuBar>
#include <QMenu>
#include <QtTest/QTest>
#include <QtCore/QEvent>

#include "./qrgui/mainWindow/mainWindow.h"
#include "./qrgui/mainWindow/palette/draggableElement.h"

using namespace qReal;
using namespace gui;

GuiFacade::GuiFacade(MainWindow &mainWindow)
	: mMainWindow(mainWindow)
{
}

QWidget *GuiFacade::widget(const QString &type, const QString &name) const
{
	return utils::WidgetFinder::widget(&mMainWindow, type, name);
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
	const PropertyEditorView * const propertyEditor = mMainWindow.propertyEditor();
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

QWidget *GuiFacade::viewPort(const QAbstractScrollArea *widget) const
{
	return widget->viewport();
}

bool GuiFacade::isEnabledAndVisible(const QAction *action) const
{
	return action->isEnabled() && action->isVisible();
}

bool GuiFacade::isEnabledAndVisible(const QWidget *widget) const
{
	return widget->isEnabled() && widget->isVisible();
}

QAction *GuiFacade::getActionInMenu(const QMenu *menu, const QString &actionName) const
{
	QList<QAction *> actions = menu->actions();
	for (QAction * const action: actions) {
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

QMenu *GuiFacade::getMenuContainedByAction(QAction *action) const
{
	emit action->menu()->aboutToShow();
	return action->menu();
}

// Для корректной работы QTest::keyClick в qt 5.5 необходимо вводить char (русские символы не работают)
// ASSERT: "false" in file qasciikey.cpp, line 222
// в будущем, если придется через гуи тестить русский С, то придется с этим что-то придумать
// сейчас в настройках запуска для тестов устанавливается параметр --no-locale
QMenu *GuiFacade::getMenu(const QString &menuName) const
{
	QMenuBar *menuBar = mMainWindow.findChild<QMenuBar *>();
	QMenu *menu = menuBar->findChild<QMenu *>(menuName, Qt::FindDirectChildrenOnly);
	return menu;
}

void GuiFacade::activateMenu(QMenu *menu)
{
	QTest::keyClick(&mMainWindow, menu->title().at(1).toLatin1(), Qt::AltModifier);
}

void GuiFacade::activateMenuAction(QMenu *menu, QAction *actionForExec)
{
	for (const QAction *action : menu->actions()) {
		if (action == actionForExec) {
			QTest::keyClick(menu, Qt::Key_Enter);
			return;
		}
		if (!action->isSeparator()) {
			QTest::qWait(50);
			QTest::keyClick(menu, Qt::Key_Down);
		}
	}

// тут наброски кода для нажатия мышкой, а надо это вообще? (может, в отдельный метод)
//	QRect actionRect = menu->actionGeometry(action);
//	QMouseEvent *mouseEvent = new QMouseEvent(QEvent::MouseButtonDblClick
//											  , menu->mapFrom(menu->parentWidget(), actionRect.topLeft())
//											  , Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
//	QApplication::postEvent(&mMainWindow, mouseEvent);
}

// быть может понадобится, если я буду выносить отдельно кликание
//QRect GuiFacade::actionRect(QMenu *menu, QAction *action) const
//{
//	return menu->setAct
//}

bool GuiFacade::actionIsChecked(const QAction *action) const
{
	return action->isChecked();
}

bool GuiFacade::actionIsCheckable(const QAction *action) const
{
	return action->isCheckable();
}

QWidget *GuiFacade::getStartButton(const QString &buttonText) const
{
	const QList<QPushButton *> allButtons = mMainWindow.findChildren<QPushButton *>();
	for (QPushButton * const button : allButtons) {
		if (button->isVisible() && button->objectName() == buttonText) {
			return button;
		}
	}
	return nullptr;
}

QPoint GuiFacade::topLeftWidgetCorner(const QWidget *widget) const
{
	return widget->pos();
}

void GuiFacade::printInfo() const
{
	QList<DraggableElement *> const paletteWidgets = mMainWindow.findChildren<DraggableElement *>();
	for (DraggableElement * const paletteElement : paletteWidgets) {
		qDebug() << paletteElement->id().toString();
	}
}
