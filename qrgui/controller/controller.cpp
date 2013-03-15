#include "controller.h"

using namespace qReal;
using namespace qReal::commands;

void Controller::setActiveDiagram(Id const &diagramId)
{
	if (diagramId != Id()) {
		setActiveStack(mStacks[diagramId.toString()]);
	} else {
		setActiveStack(NULL);
	}
}

void Controller::execute(commands::AbstractCommand *command)
{
	UndoStack *activeStack = activeUndoStack();
	if (activeStack) {
		activeStack->execute(command);
	}
}

void Controller::execute(commands::AbstractCommand *command, Id const &diagramid)
{
	UndoStack *activeStack = mStacks[diagramid.toString()];
	if (activeStack) {
		activeStack->execute(command);
	}
}

void Controller::diagramOpened(Id const &diagramId)
{
	if (diagramId == Id()) {
		return;
	}
	UndoStack *stack = new UndoStack;
	mStacks.insert(diagramId.toString(), stack);
	addStack(stack);
}

void Controller::diagramClosed(Id const &diagramId)
{
	if (diagramId == Id() || !mStacks.keys().contains(diagramId.toString())) {
		return;
	}
	UndoStack *stackToRemove = mStacks[diagramId.toString()];
	mStacks.remove(diagramId.toString());
	removeStack(stackToRemove);
}

QUndoStack *Controller::activeStack() const
{
	return QUndoGroup::activeStack();
}

void Controller::addStack(QUndoStack *stack)
{
	QUndoGroup::addStack(stack);
}

void Controller::setActiveStack(QUndoStack *stack)
{
	QUndoGroup::setActiveStack(stack);
}

QList<QUndoStack *> Controller::stacks() const
{
	return QUndoGroup::stacks();
}

QString Controller::undoText() const
{
	return QUndoGroup::undoText();
}

QString Controller::redoText() const
{
	return QUndoGroup::redoText();
}

void Controller::removeStack(QUndoStack *stack)
{
	QUndoGroup::removeStack(stack);
}

UndoStack *Controller::activeUndoStack() const
{
	return dynamic_cast<UndoStack *>(activeStack());
}
