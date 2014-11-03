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
	void setActiveDiagram(Id const &diagramId);

	/// Executes given command regarding it binded to active diagram
	void execute(commands::AbstractCommand *command);

	/// Executes given command regarding it binded to specified diagram
	void execute(commands::AbstractCommand *command, Id const &diagramid);

	/// Executes given command regarding it binded to global application space.
	/// Using this method supposes that @param command is not binded to any diagram
	/// (for example, removing diagrams themselves from model explorers)
	void executeGlobal(commands::AbstractCommand *command);

	/// Tells controller that user opened diagram with specified id
	void diagramOpened(Id const &diagramId);

	/// Tells controller that user closed diagram with specified id
	void diagramClosed(Id const &diagramId);

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
	void connectStack(UndoStack const *stack);
	void disconnectStack(UndoStack const *stack);

	void execute(commands::AbstractCommand *command, UndoStack *stack);

	UndoStack *mGlobalStack;
	UndoStack *mActiveStack;
	QMap<QString, UndoStack *> mDiagramStacks;
	bool mModifiedState;
	bool mCanRedoState;
	bool mCanUndoState;
};

}
