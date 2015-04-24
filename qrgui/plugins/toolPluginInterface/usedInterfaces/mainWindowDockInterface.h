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

#pragma once

#include <QtWidgets/QDockWidget>

namespace qReal
{
namespace gui
{

class MainWindowDockInterface
{
public:
	virtual ~MainWindowDockInterface() {}

	/// Returns the pointer to main window`s logical model explorer dock
	virtual QDockWidget *logicalModelDock() const = 0;

	/// Returns the pointer to main window`s graphical model explorer dock
	virtual QDockWidget *graphicalModelDock() const = 0;

	/// Returns the pointer to main window`s property editor dock
	virtual QDockWidget *propertyEditorDock() const = 0;

	/// Returns the pointer to main window`s error reporter dock
	virtual QDockWidget *errorReporterDock() const = 0;

	/// Returns the pointer to main window`s palette dock
	virtual QDockWidget *paletteDock() const = 0;

	/// Moves @param second dock widget on top of @param first dock widget, creating a tabbed dock area
	/// in the main window
	virtual void tabifyDockWidget(QDockWidget *first, QDockWidget *second) = 0;

	/// Adds the given @param dockWidget into the specified @param area
	virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockWidget) = 0;
};

}
}
