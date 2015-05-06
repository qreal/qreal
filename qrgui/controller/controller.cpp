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

#include "controller.h"

using namespace qReal;
using namespace qReal::commands;

Controller::Controller()
	: mGlobalStack(new UndoStack)
	, mActiveStack(nullptr)
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

void Controller::setActiveDiagram(const Id &diagramId)
{
	if (diagramId != Id()) {
		setActiveStack(mDiagramStacks[diagramId.toString()]);
	} else {
		setActiveStack(nullptr);
	}
	resetAll();
}

void Controller::execute(commands::AbstractCommand *command)
{
	execute(command, mActiveStack);
}

void Controller::execute(commands::AbstractCommand *command, const Id &diagramid)
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

void Controller::diagramOpened(const Id &diagramId)
{
	if (diagramId.isNull()) {
		return;
	}
	UndoStack *stack = new UndoStack;
	connectStack(stack);
	mDiagramStacks.insert(diagramId.toString(), stack);
	resetAll();
}

void Controller::diagramClosed(const Id &diagramId)
{
	if (diagramId.isNull() || !mDiagramStacks.keys().contains(diagramId.toString())) {
		return;
	}

	if (mActiveStack == mDiagramStacks[diagramId.toString()]) {
		mActiveStack = nullptr;
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
	const bool canUndo = (mActiveStack && mActiveStack->canUndo()) || mGlobalStack->canUndo();
	if (canUndo != mCanUndoState) {
		mCanUndoState = canUndo;
		emit canUndoChanged(mCanUndoState);
	}
}

void Controller::resetCanRedoState()
{
	const bool canRedo = (mActiveStack && mActiveStack->canRedo()) || mGlobalStack->canRedo();
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
	const int shift = forUndo ? -1 : 0;
	const int diagramIndex = mActiveStack ? mActiveStack->index() + shift : -1;
	const int globalIndex = mGlobalStack->index() + shift;
	const AbstractCommand *diagramCommand = diagramIndex < 0 ? nullptr
			: dynamic_cast<const AbstractCommand *>(mActiveStack->command(diagramIndex));
	const AbstractCommand *globalCommand = globalIndex < 0 ? nullptr
			: dynamic_cast<const AbstractCommand *>(mGlobalStack->command(globalIndex));
	if (!diagramCommand && !globalCommand) {
		return nullptr;
	}
	if (!diagramCommand) {
		return mGlobalStack;
	}
	if (!globalCommand) {
		return mActiveStack;
	}
	const uint diagramTimestamp = diagramCommand->timestamp();
	const uint globalTimestamp = globalCommand->timestamp();
	return forUndo == (diagramTimestamp < globalTimestamp) ? mGlobalStack : mActiveStack;
}

void Controller::setActiveStack(UndoStack *stack)
{
	mActiveStack = stack;
}

void Controller::connectStack(const UndoStack *stack)
{
	connect(stack, SIGNAL(cleanChanged(bool)), this, SLOT(resetModifiedState()));
	connect(stack, SIGNAL(canRedoChanged(bool)), this, SLOT(resetCanRedoState()));
	connect(stack, SIGNAL(canUndoChanged(bool)), this, SLOT(resetCanUndoState()));
}
