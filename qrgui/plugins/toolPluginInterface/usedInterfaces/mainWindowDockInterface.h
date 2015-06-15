/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
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

#include <QtWidgets/QDockWidget>

class QStatusBar;
class QToolBar;

namespace qReal {
namespace gui {

class MainWindowDockInterface
{
public:
	virtual ~MainWindowDockInterface() {}

	/// Returns a widget of main window for such operation as appending actions to it and so on.
	virtual QWidget *windowWidget() = 0;

	/// Returns the pointer to main window`s logical model explorer dock.
	virtual QDockWidget *logicalModelDock() const = 0;

	/// Returns the pointer to main window`s graphical model explorer dock.
	virtual QDockWidget *graphicalModelDock() const = 0;

	/// Returns the pointer to main window`s property editor dock.
	virtual QDockWidget *propertyEditorDock() const = 0;

	/// Returns the pointer to main window`s error reporter dock.
	virtual QDockWidget *errorReporterDock() const = 0;

	/// Returns the pointer to main window`s palette dock.
	virtual QDockWidget *paletteDock() const = 0;

	/// Returns the pointer to main window`s stripe in the bottom of the window.
	virtual QStatusBar *statusBar() const = 0;

	/// Returns a list of all toolbars on this main window.
	virtual QList<QToolBar *> toolBars() const = 0;

	/// Moves @param second dock widget on top of @param first dock widget, creating a tabbed dock area
	/// in the main window.
	virtual void tabifyDockWidget(QDockWidget *first, QDockWidget *second) = 0;

	/// Adds the given @param dockWidget into the specified @param area.
	virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockWidget) = 0;

	/// Serializes all docks and toolbar placement and returns the resulting data.
	/// This data can be used for restoring the window state next time.
	/// @param version An integer parameter that will be also serialized into state.
	virtual QByteArray saveState(int version = 0) const = 0;

	/// Restores the @arg state of the main window obtained by saveState() method.
	/// @param version An integer parameter that will be compared to the one serialized in @arg state.
	virtual bool restoreState(const QByteArray &state, int version = 0) = 0;

	/// Sets the given dock widget @arg area to occupy the specified @arg corner.
	virtual void setCorner(Qt::Corner corner, Qt::DockWidgetArea area) = 0;
};

}
}
