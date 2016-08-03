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

#include <QtCore/QMap>

#include "qrgui/controller/controllerInterface.h"
#include "qrgui/controller/undoStack.h"

namespace qReal {

/// A controller class for all user actions watching.
/// A part of global MVC architecture.
/// Module is identified with unique string. This may be root node id for diagram stacks or ids of some other editors
/// like shape editor or 2D model world editor.
class QRGUI_CONTROLLER_EXPORT Controller : public ControllerInterface
{
	Q_OBJECT

public:
	Controller();
	~Controller();

	bool canUndo() const override;
	bool canRedo() const override;

public slots:
	void setActiveModule(const QString &moduleId) override;
	void moduleOpened(const QString &moduleId) override;
	void moduleClosed(const QString &moduleId) override;

	void execute(commands::AbstractCommand *command) override;
	void execute(commands::AbstractCommand *command, const QString &moduleId) override;
	void executeGlobal(commands::AbstractCommand *command) override;

	void redo() override;
	void undo() override;

	/// Tells controller that user saved project
	void projectSaved();

	/// Tells controller that current project was closed
	void projectClosed();

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

	UndoStack *mGlobalStack;                   // Has ownership.
	UndoStack *mActiveStack;                   // Has ownership.
	QMap<QString, UndoStack *> mModuleStacks;  // Has ownership.
	bool mModifiedState;
	bool mCanRedoState;
	bool mCanUndoState;
};

}
