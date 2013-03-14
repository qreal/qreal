#include "controller.h"

using namespace qReal;
using namespace qReal::commands;

Controller::Controller()
{
}

Controller::~Controller()
{
	clear(mExecutedCommands);
	clear(mUndoneCommands);
}

bool Controller::execute(commands::AbstractCommand *command)
{
	return execPrivate(command, true);
}

bool Controller::execPrivate(commands::AbstractCommand *command, bool clearStack)
{
	bool const result = (*command)();
	if (result) {
		mExecutedCommands.push(command);
		if (clearStack) {
			clear(mUndoneCommands);
		}
	}
	return result;
}

bool Controller::undo()
{
	if (mExecutedCommands.isEmpty()) {
		return true;
	}
	AbstractCommand *lastCommand = mExecutedCommands.pop();
	bool const result = lastCommand->undo();
	if (result) {
		mUndoneCommands.push(lastCommand);
	}
	return result;
}

bool Controller::redo()
{
	if (mUndoneCommands.isEmpty()) {
		return true;
	}
	return execPrivate(mUndoneCommands.pop(), false);
}

void Controller::clear(QStack<AbstractCommand *> &stack)
{
	foreach (AbstractCommand * const command, stack) {
		delete command;
	}
	stack.clear();
}
