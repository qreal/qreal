#include "undoStack.h"

using namespace qReal;

UndoStack::UndoStack()
{
}

void UndoStack::execute(commands::AbstractCommand *command)
{
	push(command);
}
