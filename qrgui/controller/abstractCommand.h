#pragma once

namespace qReal
{
namespace commands
{

class AbstractCommand
{
public:
	AbstractCommand();

	bool operator()();
	bool undo();

protected:
	/// Returns operation success
	virtual bool execute() = 0;

	/// Implementation must undo all the changes made by this command
	/// and return operation success
	virtual bool restoreState() = 0;

private:
	bool mExecuted;
};

}
}
