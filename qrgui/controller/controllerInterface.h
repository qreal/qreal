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

#include <QtCore/QObject>

#include "qrgui/controller/controllerDeclSpec.h"
#include "qrgui/controller/commands/abstractCommand.h"

namespace qReal {

/// A controller for all user actions watching. A part of global MVC architecture.
/// Controller manager behaviour of some modules. Each module has its own undo stack that can store different commands.
/// Module is identified with unique string. This may be root node id for diagram stacks or ids of some other editors
/// like shape editor or 2D model world editor.
/// @todo: Get rid of global stack.
class QRGUI_CONTROLLER_EXPORT ControllerInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~ControllerInterface() {}

	/// Returns true if last action can be undone for current stack or global stack.
	virtual bool canUndo() const = 0;

	/// Returns true if next action can be repeater for current stack or global stack.
	virtual bool canRedo() const = 0;

public slots:
	/// Tells controller that user swithed to some module with \a moduleId.
	virtual void setActiveModule(const QString &moduleId) = 0;

	/// Executes given command regarding it binded to active module.
	virtual void execute(commands::AbstractCommand *command) = 0;

	/// Executes given command regarding it binded to specified module.
	virtual void execute(commands::AbstractCommand *command, const QString &moduleId) = 0;

	/// Executes given command regarding it binded to global application space.
	/// Using this method supposes that @param command is not binded to any module
	/// (for example, removing diagrams themselves from model explorers).
	virtual void executeGlobal(commands::AbstractCommand *command) = 0;

	/// Tells controller that user started working with module with \a moduleId.
	virtual void moduleOpened(const QString &moduleId) = 0;

	/// Tells controller that user stopped working with module with \a moduleId.
	virtual void moduleClosed(const QString &moduleId) = 0;

	/// Invokes the latest command on one of the stacks: active module stack or global one.
	virtual void redo() = 0;

	/// Rolls back the earliest command on one of the stacks: active module stack or global one.
	virtual void undo() = 0;

signals:
	/// Emitted when at least one of the stacks has modifications.
	void modifiedChanged(bool modified);

	/// Emitted whenever the value of canUndo() changes.
	void canUndoChanged(bool canUndo);

	/// Emitted whenever the value of canRedo() changes.
	void canRedoChanged(bool canRedo);
};

}
