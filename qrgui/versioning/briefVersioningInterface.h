#pragma once

#include <QtCore/QString>

namespace qReal
{
namespace versioning
{

/// Provides a number of actions that each VCS should be
/// able to invoke by definition.
class BriefVersioningInterface
{
public:
	virtual ~BriefVersioningInterface() {}

	/// Tries to fetch working copy from specified repo into specified project.
	/// Examples: svn checkout, git clone, etc...
	/// @param repoAddress Repository URL
	/// @param targetProject Path to target project
	/// @param revisionNumber Order number of the required revision. If negative value specified, fetching last revision.
	virtual bool downloadWorkingCopy(QString const &repoAddress
			, QString const &targetProject
			, int revisionNumber = -1) = 0;

	/// Tries to update specified project to last revision.
	/// Examples: svn update, git pull, etc...
	virtual bool updateWorkingCopy() = 0;

	/// Tries to apply changes made in specified project to repository.
	/// Examples: svn commit, git commit + git push, etc...
	/// @param description A message that desciripts all changes made in working copy
	virtual bool submitChanges(QString const &description) = 0;

	/// Invokes internal operations for reiniting itself.
	/// Examples: svn clean up, git init
	virtual bool reinitWorkingCopy() = 0;

	/// Returns information about versioning of specified project.
	/// The result format is specific for each VCS and it can return
	/// info about different details.
	/// Examples: svn info
	virtual QString information() = 0;

	/// Returns the order number of current working copy revision
	virtual int revisionNumber() = 0;

	/// Implementation must determine if specified working copy
	/// is versioned with this VCS.
	/// @param directory Path to interested directory
	virtual bool isMyWorkingCopy(QString const &directory = "") = 0;

};

}
}
