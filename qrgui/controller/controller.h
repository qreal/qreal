#pragma once

#include <QtCore/QObject>
#include <QtCore/QStack>

#include "commands/abstractCommand.h"

namespace qReal
{

class Controller : public QObject
{
	Q_OBJECT

public:
	Controller();
	~Controller();

public slots:
	/// Executes @param command and takes ownership on it
	bool execute(commands::AbstractCommand *command);

	/// Cancelles last action if exists
	bool undo();

	/// Invokes last cancelled action if exists
	bool redo();

private:
	bool execPrivate(commands::AbstractCommand *command, bool clearStack);

	void clear(QStack<commands::AbstractCommand *> &stack);

	QStack<commands::AbstractCommand *> mExecutedCommands;
	QStack<commands::AbstractCommand *> mUndoneCommands;
};

}
