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
	for (UndoStack * const stack : mModuleStacks) {
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

void Controller::setActiveModule(const QString &moduleId)
{
	if (moduleId.isEmpty()) {
		setActiveStack(nullptr);
	} else {
		setActiveStack(mModuleStacks[moduleId]);
	}

	resetAll();
}

void Controller::execute(commands::AbstractCommand *command)
{
	execute(command, mActiveStack);
}

void Controller::execute(commands::AbstractCommand *command, const QString &moduleId)
{
	execute(command, mModuleStacks[moduleId]);
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

void Controller::moduleOpened(const QString &moduleId)
{
	if (moduleId.isEmpty()) {
		return;
	}

	UndoStack *stack = new UndoStack;
	connectStack(stack);
	mModuleStacks.insert(moduleId, stack);
	resetAll();
}

void Controller::moduleClosed(const QString &moduleId)
{
	if (moduleId.isEmpty() || !mModuleStacks.keys().contains(moduleId)) {
		return;
	}

	if (mActiveStack == mModuleStacks[moduleId]) {
		mActiveStack = nullptr;
	}

	delete mModuleStacks[moduleId];
	mModuleStacks.remove(moduleId);
	resetAll();
}

void Controller::resetModifiedState()
{
	bool wasModified = false;
	QList<UndoStack *> const undoStacks = stacks();
	for (UndoStack * const stack : undoStacks) {
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
	return mModuleStacks.values() << mGlobalStack;
}

void Controller::projectSaved()
{
	mGlobalStack->setClean();
	foreach (UndoStack * const stack, mModuleStacks) {
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
	const int moduleIndex = mActiveStack ? mActiveStack->index() + shift : -1;
	const int globalIndex = mGlobalStack->index() + shift;
	const AbstractCommand *moduleCommand = moduleIndex < 0 ? nullptr
			: dynamic_cast<const AbstractCommand *>(mActiveStack->command(moduleIndex));
	const AbstractCommand *globalCommand = globalIndex < 0 ? nullptr
			: dynamic_cast<const AbstractCommand *>(mGlobalStack->command(globalIndex));
	if (!moduleCommand && !globalCommand) {
		return nullptr;
	}
	if (!moduleCommand) {
		return mGlobalStack;
	}
	if (!globalCommand) {
		return mActiveStack;
	}
	const uint moduleTimestamp = moduleCommand->timestamp();
	const uint globalTimestamp = globalCommand->timestamp();
	return forUndo == (moduleTimestamp < globalTimestamp) ? mGlobalStack : mActiveStack;
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
