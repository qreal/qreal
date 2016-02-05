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

#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QScrollBar>

#include <plugins/toolPluginInterface/toolPluginInterface.h>

namespace qReal {

class MainWindow;

namespace gui {

class DraggableElement;

/// A facade object providing a convent way to access different elements of QReal GUI in scripts.
class GuiFacade : public QObject
{
	Q_OBJECT

public:
	explicit GuiFacade(MainWindow &mainWindow);

	/// @returns main window child widget by its type (class name) and QObject name.
	/// @note Doesnt work for QMenu. Use getMenu() instead.
	/// Dont forget about scrollViewPorts in some cases.
	Q_INVOKABLE QWidget *widget(const QString &type, const QString &objectName = QString()) const;

	/// @returns main window child widget by its type (class name) and QObject name having \a widget in the parent list.
	/// @see qReal::gui::GuiFacade::widget for more information.
	Q_INVOKABLE QWidget *widgetClarified(const QWidget *parent
			, const QString &type, const QString &objectName = QString()) const;

	/// @returns a widget of some action on the toolbar panel.
	Q_INVOKABLE QWidget *pluginActionToolButton(const QString &name) const;

	/// @returns a bounding rectangle of some property`s interactive region in property editor in global coordinates.
	/// This can be used for emulating mouse click into this region to instantiate the editor widget.
	Q_INVOKABLE QRect propertyRect(const QString &name) const;

	/// @returns widget by name from property editor tree widget. The widget must be instantiated first
	/// by clicking the property editor into the property rectangle.
	/// @see qReal::gui::GuiFacade::propertyRect.
	Q_INVOKABLE QWidget *property(const QString &name) const;

	/// @returns the target for all mouse events on the scene of the currently opened editor tab.
	/// If no tab opened or non-editor tab is selected nullptr is returned.
	Q_INVOKABLE QWidget *sceneViewport() const;

	/// @returns a reference to the main window widget.
	Q_INVOKABLE QWidget *mainWindow() const;

	/// @returns the target for all mouse events on property editor widget.
	Q_INVOKABLE QWidget *propertyEditor() const;

	/// @returns the widget that can be dragged out of the palette to create element on the scene.
	/// @param widgetId Element metatype string, same as Id::toString() will return for this element type.
	DraggableElement *draggableElement(const QString &widgetId) const;

	/// @returns facade object of some tool plugin. This object will provide plugin`s parts in plugin`s terms.
	/// @param: pluginName The name of the plugin specified in its metadata in IID section.
	QObject *pluginGuiFacade(const QString &pluginName) const;

	/// @returns first available viewport widget pointer with "qt_scrollarea_viewport" name,
	/// or 0 if there is no such object. The search is performed recursively.
	/// If there is more than one child matching the search, the most direct ancestor is returned.
	/// If there are several direct ancestors, it is undefined which one will be returned.
	/// \a object must be not 0.
	Q_INVOKABLE QWidget *deepViewPort(const QObject *object) const;

	/// @returns viewport widget pointer of this \a widget.
	Q_INVOKABLE QWidget *viewPort(const QAbstractScrollArea *widget) const;

	/// @returns corresponding menu pointer from the main menuBar, nullptr if such main doesnt exist.
	/// @note Looks only at the direct children of the object.
	Q_INVOKABLE QMenu *getMenu(const QString &menuName) const;

	/// @returns corresponding action pointer from the main menuBar, nullptr if such action doesnt exist.
	/// @note Looking for by a text or an object name.
	Q_INVOKABLE QAction *getActionInMenu(const QMenu *menu, const QString &actionName) const;

	/// Need repeatedly to call this method (if the menu is opening again) because
	/// this method emits necessary signal (see warning).
	/// @warning it needs because the appropriative bug exists: https://bugs.launchpad.net/appmenu-qt5/+bug/1449373.
	Q_INVOKABLE QMenu *getMenuContainedByAction(QAction *action) const;

	/// @returns true if \a action is a submenu in \a menu.
	Q_INVOKABLE bool isSubMenuInMenu(const QMenu *menu, const QAction *action) const;

	/// @returns the widget pointer of visible push button corresponding to \a buttonText and startWidget,
	/// otherwise returns nullptr.
	Q_INVOKABLE QWidget *getStartButton(const QString &buttonText) const;

private:
	QTreeWidgetItem *propertyTreeWidgetItem(const QString &name) const;

	MainWindow &mMainWindow;
};
}
}
