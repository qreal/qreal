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

#include <qrkernel/ids.h>

#include "qrgui/controller/controllerDeclSpec.h"
#include "qrgui/controller/undoStack.h"
#include "qrgui/controller/commands/abstractCommand.h"

namespace qReal {

/// A controller class for all user actions watching.
/// A part of global MVC architecture.
class QRGUI_CONTROLLER_EXPORT Controller : public QObject
{
	Q_OBJECT

public:
	Controller();
	virtual ~Controller();

	bool canUndo() const;
	bool canRedo() const;

public slots:
	/// Tells controller that user swithed to diagram with specified id as root
	void setActiveDiagram(const Id &diagramId);

	/// Executes given command regarding it binded to active diagram
	void execute(commands::AbstractCommand *command);

	/// Executes given command regarding it binded to specified diagram
	void execute(commands::AbstractCommand *command, const Id &diagramid);

	/// Executes given command regarding it binded to global application space.
	/// Using this method supposes that @param command is not binded to any diagram
	/// (for example, removing diagrams themselves from model explorers)
	void executeGlobal(commands::AbstractCommand *command);

	/// Tells controller that user opened diagram with specified id
	void diagramOpened(const Id &diagramId);

	/// Tells controller that user closed diagram with specified id
	void diagramClosed(const Id &diagramId);

	/// Tells controller that user saved project
	void projectSaved();

	/// Tells controller that current project was closed
	void projectClosed();

	/// Invokes the latest command on one of the stacks: active diagram stack or global one
	void redo();

	/// Rolls back the earliest command on one of the stacks: active diagram stack or global one
	void undo();

signals:
	/// Emitted when at least one of the stacks has modifications
	void modifiedChanged(bool modified);

	/// Emitted whenever the value of canUndo() changes
	void canUndoChanged(bool canUndo);

	/// Emitted whenever the value of canRedo() changes
	void canRedoChanged(bool canRedo);

private slots:
	void resetModifiedState();
	void resetCanRedoState();
	void resetCanUndoState();
	void resetAll();

private:
	QList<UndoStack *> stacks() const;
	UndoStack *selectActiveStack(bool forUndo);

	void setActiveStack(UndoStack *stack);
	void connectStack(const UndoStack *stack);
	void disconnectStack(const UndoStack *stack);

	void execute(commands::AbstractCommand *command, UndoStack *stack);

	UndoStack *mGlobalStack;
	UndoStack *mActiveStack;
	QMap<QString, UndoStack *> mDiagramStacks;
	bool mModifiedState;
	bool mCanRedoState;
	bool mCanUndoState;
};

}
