#include "doNothingCommand.h"

using namespace qReal::commands;

DoNothingCommand::DoNothingCommand()
{
}

bool DoNothingCommand::execute()
{
	return true;
}

bool DoNothingCommand::restoreState()
{
	return true;
}
