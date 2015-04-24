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

#include <QtCore/QListIterator>
#include <QtCore/QDateTime>

#include "abstractCommand.h"

using namespace qReal::commands;

AbstractCommand::AbstractCommand()
	: mExecuted(false), mRedoEnabled(true), mUndoEnabled(true)
{
	mTimestamp = QDateTime::currentMSecsSinceEpoch();
}

AbstractCommand::~AbstractCommand()
{
	foreach (AbstractCommand *command, mPreActions) {
		delete command;
	}
	foreach (AbstractCommand *command, mPostActions) {
		delete command;
	}
}

void AbstractCommand::redo()
{
	if (!mRedoEnabled) {
		mExecuted = true;
		return;
	}
	if (mExecuted) {
		return;
	}
	executeDirect(mPreActions);
	mExecuted = execute();
	executeDirect(mPostActions);
	emit redoComplete(mExecuted);
}

void AbstractCommand::undo()
{
	if (!mUndoEnabled) {
		mExecuted = false;
		return;
	}
	if (!mExecuted) {
		return;
	}
	executeReverse(mPostActions);
	mExecuted = !restoreState();
	executeReverse(mPreActions);
	emit undoComplete(!mExecuted);
}

void AbstractCommand::setRedoEnabled(bool enabled)
{
	mRedoEnabled = enabled;
}

void AbstractCommand::setUndoEnabled(bool enabled)
{
	mUndoEnabled = enabled;
}

void AbstractCommand::addPreAction(AbstractCommand * const command)
{
	if (command) {
		mPreActions << command;
	}
}

void AbstractCommand::insertPreAction(AbstractCommand * const command, int index)
{
	if (command) {
		mPreActions.insert(index, command);
	}
}

void AbstractCommand::addPostAction(AbstractCommand * const command)
{
	if (command) {
		mPostActions << command;
	}
}

void AbstractCommand::insertPostAction(AbstractCommand * const command, int index)
{
	if (command) {
		mPostActions.insert(index, command);
	}
}

void AbstractCommand::executeDirect(QList<AbstractCommand *> const &list)
{
	QListIterator<AbstractCommand *> iterator(list);
	while (iterator.hasNext()) {
		iterator.next()->redo();
	}
}

void AbstractCommand::executeReverse(QList<AbstractCommand *> const &list)
{
	QListIterator<AbstractCommand *> iterator(list);
	iterator.toBack();
	while (iterator.hasPrevious()) {
		iterator.previous()->undo();
	}
}

bool AbstractCommand::hierarchyContains(AbstractCommand * const command) const
{
	if ((*this == *command && this != command)
			|| contains(mPreActions, command)
			|| contains(mPostActions, command)) {
		return true;
	}
	foreach (AbstractCommand * const childCommand, mPreActions) {
		if (childCommand->hierarchyContains(command)) {
			return true;
		}
	}
	foreach (AbstractCommand * const childCommand, mPostActions) {
		if (childCommand->hierarchyContains(command)) {
			return true;
		}
	}
	return false;
}

bool AbstractCommand::contains(QList<AbstractCommand *> const &list
		, const AbstractCommand *command) const
{
	foreach (AbstractCommand * const otherCommand, list) {
		if (*otherCommand == *command && otherCommand != command) {
			return true;
		}
	}
	return false;
}

bool AbstractCommand::equals(const AbstractCommand &other) const
{
	Q_UNUSED(other)
	return &other == this;
}

void AbstractCommand::removeDuplicates()
{
	removeDuplicatesOn(mPreActions);
	removeDuplicatesOn(mPostActions);
}

qReal::Id AbstractCommand::diagramBinded() const
{
	return mDiagramBinded;
}

void AbstractCommand::bindToDiagram(const qReal::Id &diagramId)
{
	mDiagramBinded = diagramId;
}

uint AbstractCommand::timestamp() const
{
	return mTimestamp;
}

void AbstractCommand::removeDuplicatesOn(QList<AbstractCommand *> &list)
{
	foreach (AbstractCommand * const command, list) {
		if (hierarchyContains(command)) {
			list.removeAll(command);
			delete command;
		} else {
			command->removeDuplicates();
		}
	}
}
