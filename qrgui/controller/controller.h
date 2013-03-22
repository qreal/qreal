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
	Controller();

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

	/// Tells controller that user saved project
	void projectSaved();

signals:
	/// Emitted when at least one of the stacks has modifications
	void modifiedChanged(bool modified);

private slots:
	void resetModifiedState();

private:
	void execute(commands::AbstractCommand *command, UndoStack *stack);

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
	bool mModifiedState;
};

}
