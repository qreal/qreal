#include "abstractCommand.h"

using namespace qReal::commands;

AbstractCommand::AbstractCommand()
	: mExecuted(false)
{
}

bool AbstractCommand::operator()()
{
	if (mExecuted) {
		return false;
	}
	mExecuted = execute();
	return mExecuted;
}

bool AbstractCommand::undo()
{
	if (!mExecuted) {
		return false;
	}
	mExecuted = !restoreState();
	return !mExecuted;
}
