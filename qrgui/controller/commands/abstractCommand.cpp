#include <QtCore/QListIterator>

#include "abstractCommand.h"

using namespace qReal::commands;

AbstractCommand::AbstractCommand()
	: mExecuted(false), mRedoEnabled(true), mUndoEnabled(true)
{
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
		, AbstractCommand const *command) const
{
	foreach (AbstractCommand * const otherCommand, list) {
		if (*otherCommand == *command && otherCommand != command) {
			return true;
		}
	}
	return false;
}

bool AbstractCommand::equals(AbstractCommand const &other) const
{
	Q_UNUSED(other)
	return &other == this;
}

void AbstractCommand::removeDuplicates()
{
	removeDuplicatesOn(mPreActions);
	removeDuplicatesOn(mPostActions);
}

void AbstractCommand::removeDuplicatesOn(QList<AbstractCommand *> &list)
{
	foreach (AbstractCommand * const command, list) {
		if (hierarchyContains(command)) {
			list.removeAll(command);
			delete command;
		}
	}
}
