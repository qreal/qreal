#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>

namespace qReal
{
namespace versioning
{

/// Provides a number of actions that each VCS should be
/// able to invoke by definition.
class BriefVersioningInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~BriefVersioningInterface() {}

public slots:
	/// Starts fetching working copy from specified repo into specified project
	/// in asynchroniouos mode. When finished emitted @see workingCopyDownloaded();
	/// Examples: svn checkout, git clone, etc...
	/// @param repoAddress Repository URL
	/// @param targetProject Path to target project
	/// @param revisionNumber Order number of the required revision. If negative value specified, fetching last revision.
	virtual void beginWorkingCopyDownloading(QString const &repoAddress
			, QString const &targetProject
			, int revisionNumber = -1) = 0;

	/// Starts updating specified project to last revision
	/// in asynchroniouos mode. When finished emitted @see
	/// Examples: svn update, git pull, etc...
	virtual void beginWorkingCopyUpdating() = 0;

	/// Starts applying changes made in specified project to remote repository
	/// in asynchroniouos mode. When finished emitted @see
	/// Examples: svn commit, git commit + git push, etc...
	/// @param description A message that desciripts all changes made in working copy
	virtual void beginChangesSubmitting(QString const &description) = 0;

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

signals:
	/// Emitted when working copy downloading started by
	/// @see beginWorkingCopyDownloading() complete
	/// @param was operation successfull or not
	void workingCopyDownloaded(bool const success);

	/// Emitted when working copy updating started by
	/// @see beginWorkingCopyUpdating() complete
	/// @param was operation successfull or not
	void workingCopyUpdated(const bool success);

	/// Emitted when working copy changes submitting started by
	/// @see beginChangesSubmitting() complete
	/// @param was operation successfull or not
	void changesSubmitted(const bool success);
};

}
}
