#pragma once

#include "controller/commands/abstractCommand.h"

namespace qReal {
namespace commands {

/// This command does nothing. Can be useful for creating "container" commands
/// having no actions itself
class DoNothingCommand : public AbstractCommand
{
public:
	DoNothingCommand();

protected:
	virtual bool execute();
	virtual bool restoreState();
};

}
}
