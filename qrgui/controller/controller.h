#pragma once

#include <QtCore/QStack>
#include <QtGui/QUndoGroup>

#include "undoStack.h"
#include "commands/abstractCommand.h"
#include "../../qrkernel/ids.h"

namespace qReal
{

/// A controller class for all user actions watching.
/// A part of global MVC architecture.
class Controller : public QUndoGroup
{
	Q_OBJECT

public:

public slots:
	/// Tells controller that user swithed to diagram with specified id as root
	void setActiveDiagram(Id const &diagramId);

	/// Executes given command for active diagram
	void execute(commands::AbstractCommand *command);

	/// Executes given command for specified diagram
	void execute(commands::AbstractCommand *command, Id const &diagramid);

	/// Tells controller that user opened diagram with specified id
	void diagramOpened(Id const &diagramId);

	/// Tells controller that user closed diagram with specified id
	void diagramClosed(Id const &diagramId);

private:
	UndoStack *activeUndoStack() const;

	// Hiding some QUndoGroup functions
	QUndoStack *activeStack() const;
	void addStack(QUndoStack *stack);
	void setActiveStack(QUndoStack *stack);
	QList<QUndoStack *> stacks() const;
	QString undoText() const;
	QString redoText() const;
	void removeStack(QUndoStack *stack);

	QMap<QString, UndoStack *> mStacks;
};

//class Controller : public QObject
//{
//	Q_OBJECT

//public:
//	Controller();
//	~Controller();

//public slots:
//	/// Nullifies undo and redo stacks and save markers
//	void reset();


//signals:
//	/// Emitted when undo stack becomes empty or opposite
//	void canUndoChanged(bool canUndo);

//	/// Emitted when redo stack becomes empty or opposite
//	void canRedoChanged(bool canUndo);

//	/// Emitted when undo stack reaches last save marker
//	void modifiedChanged(bool modified);

//private:
//	bool execPrivate(commands::AbstractCommand *command, bool clearStack);

//	void clear(QStack<commands::AbstractCommand *> &stack);

//	QStack<commands::AbstractCommand *> mExecutedCommands;
//	QStack<commands::AbstractCommand *> mUndoneCommands;
//};

}
