#pragma once

#include <QtWidgets/QUndoStack>

#include "commands/abstractCommand.h"

namespace qReal
{

class UndoStack : public QUndoStack
{
public:
	UndoStack();

	/// Executes @param command and takes ownership on it
	void execute(commands::AbstractCommand *command);
};

}
