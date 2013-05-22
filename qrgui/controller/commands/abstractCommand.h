#pragma once

#include <QtCore/QList>
#include <QtWidgets/QUndoCommand>

namespace qReal
{
namespace commands
{

class AbstractCommand : public QUndoCommand
{
public:
	AbstractCommand();
	virtual ~AbstractCommand();

	void redo();
	void undo();

	void setRedoEnabled(bool enabled);
	void setUndoEnabled(bool enabled);

	/// Adds @param command into a list of commands to be executed
	/// before this command
	void addPreAction(AbstractCommand * const command);

	/// Adds @param command into a specified place of the list of commands
	/// to be executed before this command
	void insertPreAction(AbstractCommand * const command, int index);

	/// Adds @param command into a list of commands to be executed
	/// after this command
	void addPostAction(AbstractCommand * const command);

	/// Adds @param command into a specified place of the list of commands
	/// to be executed after this command
	void insertPostAction(AbstractCommand * const command, int index);

	virtual bool equals(AbstractCommand const &other) const;

	/// Performs command tree clearing with all duplicates to be removed.
	/// Removes duplicate closer to root, with search only in current subtree.
	/// Root command is never removed
	void removeDuplicates();

protected:
	/// Returns operation success
	virtual bool execute() = 0;

	/// Implementation must undo all the changes made by this command
	/// and return operation success
	virtual bool restoreState() = 0;

private:
	void executeDirect(QList<AbstractCommand *> const &list);
	void executeReverse(QList<AbstractCommand *> const &list);

	/// Tells if command tree already contains specified command.
	/// The command itself is not considered
	bool hierarchyContains(AbstractCommand * const command) const;

	/// The command itself is not considered
	bool contains(QList<AbstractCommand *> const &list, AbstractCommand const * command) const;

	void removeDuplicatesOn(QList<AbstractCommand *> &list);

	bool mExecuted;
	bool mRedoEnabled;
	bool mUndoEnabled;
	QList<AbstractCommand *> mPreActions;
	QList<AbstractCommand *> mPostActions;
};

inline bool operator==(AbstractCommand const &c1, AbstractCommand const &c2)
{
	return c1.equals(c2);
}

}
}
