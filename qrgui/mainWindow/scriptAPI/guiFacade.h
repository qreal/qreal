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

	/// Returns main window child widget by its type (class name) and QObject name.
	Q_INVOKABLE QWidget *widget(const QString &type, const QString &name = QString()) const;

	/// Returns a widget of some action on the toolbar panel.
	Q_INVOKABLE QWidget *pluginActionToolButton(const QString &name) const;

	/// Returns a bounding rectangle of some property`s interactive region in property editor in global ccordinates.
	/// This can be used for emulating mouse click into this region to instantiate the editor widget.
	Q_INVOKABLE QRect propertyRect(const QString &name) const;

	/// Returns widget by name from property editor tree widget. The widget must be instantiated first
	/// by clicking the property editor into the property rectangle.
	/// @see qReal::gui::GuiFacade::propertyRect.
	Q_INVOKABLE QWidget *property(const QString &name) const;

	/// Returns the target for all mouse events on the scene of the currently opened editor tab.
	/// If no tab opened or non-editor tab is selected nullptr is returned.
	Q_INVOKABLE QWidget *sceneViewport() const;

	/// Returns a reference to the main window widget.
	Q_INVOKABLE QWidget *mainWindow() const;

	/// Returns the target for all mouse events on property editor widget.
	Q_INVOKABLE QWidget *propertyEditor() const;

	/// Returns the widget that can be dragged out of the palette to create element on the scene.
	/// @param widgetId Element metatype string, same as Id::toString() will return for this element type.
	DraggableElement *draggableElement(const QString &widgetId) const;

	/// Returns facade object of some tool plugin. This object will provide plugin`s parts in plugin`s terms.
	/// @param: pluginName The name of the plugin specified in its metadata in IID section.
	QObject *pluginGuiFacade(const QString &pluginName) const;

private:
	QTreeWidgetItem *propertyTreeWidgetItem(const QString &name) const;

	MainWindow &mMainWindow;
};
}
}
