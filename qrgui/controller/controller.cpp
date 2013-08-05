#include "controller.h"

using namespace qReal;
using namespace qReal::commands;

Controller::Controller()
	: mGlobalStack(new UndoStack)
	, mActiveStack(NULL)
	, mModifiedState(false)
	, mCanRedoState(true)
	, mCanUndoState(true)
{
	connectStack(mGlobalStack);
}

Controller::~Controller()
{
	disconnect(this, SLOT(resetModifiedState()));
	disconnect(this, SLOT(resetCanRedoState()));
	disconnect(this, SLOT(resetCanUndoState()));
	foreach (UndoStack *stack, mDiagramStacks) {
		// 'delete stack;' causes segfaults when app is beeing closed:
		// for some reason slots are still connected
		stack->deleteLater();
	}
	mGlobalStack->deleteLater();
}

bool Controller::canUndo() const
{
	return mCanUndoState;
}

bool Controller::canRedo() const
{
	return mCanRedoState;
}

void Controller::setActiveDiagram(Id const &diagramId)
{
	if (diagramId != Id()) {
		setActiveStack(mDiagramStacks[diagramId.toString()]);
	} else {
		setActiveStack(NULL);
	}
	resetAll();
}

void Controller::execute(commands::AbstractCommand *command)
{
	execute(command, mActiveStack);
}

void Controller::execute(commands::AbstractCommand *command, Id const &diagramid)
{
	execute(command, mDiagramStacks[diagramid.toString()]);
}

void Controller::executeGlobal(AbstractCommand *command)
{
	execute(command, mGlobalStack);
}

void Controller::execute(commands::AbstractCommand *command, UndoStack *stack)
{
	if (command && stack) {
		stack->execute(command);
	}
}

void Controller::diagramOpened(Id const &diagramId)
{
	if (diagramId == Id()) {
		return;
	}
	UndoStack *stack = new UndoStack;
	connectStack(stack);
	mDiagramStacks.insert(diagramId.toString(), stack);
	resetAll();
}

void Controller::diagramClosed(Id const &diagramId)
{
	if (diagramId == Id() || !mDiagramStacks.keys().contains(diagramId.toString())) {
		return;
	}
	if (mActiveStack == mDiagramStacks[diagramId.toString()]) {
		mActiveStack = NULL;
	}
	delete mDiagramStacks[diagramId.toString()];
	mDiagramStacks.remove(diagramId.toString());
	resetAll();
}

void Controller::resetModifiedState()
{
	bool wasModified = false;
	QList<UndoStack *> const undoStacks = stacks();
	foreach (UndoStack *stack, undoStacks) {
		if (stack && !stack->isClean()) {
			wasModified = true;
			break;
		}
	}
	if (wasModified != mModifiedState) {
		mModifiedState = wasModified;
		emit modifiedChanged(mModifiedState);
	}
}

void Controller::resetCanUndoState()
{
	bool const canUndo = (mActiveStack && mActiveStack->canUndo()) || mGlobalStack->canUndo();
	if (canUndo != mCanUndoState) {
		mCanUndoState = canUndo;
		emit canUndoChanged(mCanUndoState);
	}
}

void Controller::resetCanRedoState()
{
	bool const canRedo = (mActiveStack && mActiveStack->canRedo()) || mGlobalStack->canRedo();
	if (canRedo != mCanRedoState) {
		mCanRedoState = canRedo;
		emit canRedoChanged(mCanRedoState);
	}
}

void Controller::resetAll()
{
	resetModifiedState();
	resetCanRedoState();
	resetCanUndoState();
}

QList<UndoStack *> Controller::stacks() const
{
	return mDiagramStacks.values() << mGlobalStack;
}

void Controller::projectSaved()
{
	mGlobalStack->setClean();
	foreach (UndoStack * const stack, mDiagramStacks) {
		stack->setClean();
	}
}

void Controller::projectClosed()
{
	mGlobalStack->clear();
}

void Controller::redo()
{
	UndoStack *stack = selectActiveStack(false);
	if (stack) {
		stack->redo();
	}
}

void Controller::undo()
{
	UndoStack *stack = selectActiveStack(true);
	if (stack) {
		stack->undo();
	}
}

UndoStack *Controller::selectActiveStack(bool forUndo)
{
	int const shift = forUndo ? -1 : 0;
	int const diagramIndex = mActiveStack ? mActiveStack->index() + shift : -1;
	int const globalIndex = mGlobalStack->index() + shift;
	AbstractCommand const *diagramCommand = diagramIndex < 0 ? NULL
			: dynamic_cast<AbstractCommand const *>(mActiveStack->command(diagramIndex));
	AbstractCommand const *globalCommand = globalIndex < 0 ? NULL
			: dynamic_cast<AbstractCommand const *>(mGlobalStack->command(globalIndex));
	if (!diagramCommand && !globalCommand) {
		return NULL;
	}
	if (!diagramCommand) {
		return mGlobalStack;
	}
	if (!globalCommand) {
		return mActiveStack;
	}
	uint const diagramTimestamp = diagramCommand->timestamp();
	uint const globalTimestamp = diagramCommand->timestamp();
	return forUndo == (diagramTimestamp < globalTimestamp) ? mGlobalStack : mActiveStack;
}

void Controller::setActiveStack(UndoStack *stack)
{
	mActiveStack = stack;
}

void Controller::connectStack(UndoStack const *stack)
{
	connect(stack, SIGNAL(cleanChanged(bool)), this, SLOT(resetModifiedState()));
	connect(stack, SIGNAL(canRedoChanged(bool)), this, SLOT(resetCanRedoState()));
	connect(stack, SIGNAL(canUndoChanged(bool)), this, SLOT(resetCanUndoState()));
}
