/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "qrutils/utilsDeclSpec.h"

namespace qReal
{

/// Provides a number of actions that each VCS should be
/// able to invoke by definition.
class QRUTILS_EXPORT BriefVersioningInterface : public QObject
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
	/// @param commitId Order id of the required revision. If negative value specified, fetching last revision.
	/// @param quiet Should system inform about the errors and open project after downloading
	virtual void beginWorkingCopyDownloading(
		QString const &repoAddress
		, QString const &targetProject
		, QString commitId = "-1"
		, bool quiet = false
	) = 0;

	/// Starts updating specified project to last revision
	/// in asynchroniouos mode. When finished emitted @see
	/// Examples: svn update, git pull, etc...
	virtual void beginWorkingCopyUpdating(QString const &targetProject = QString()) = 0;

	/// Starts applying changes made in specified project to remote repository
	/// in asynchroniouos mode. When finished emitted @see
	/// Examples: svn commit, git commit + git push, etc...
	/// @param description A message that desciripts all changes made in working copy
	virtual void beginChangesSubmitting(
		QString const &description
		, QString const &targetProject = QString()
		, bool quiet = false
	) = 0;

	/// Invokes internal operations for reiniting itself.
	/// Examples: svn clean up, git init
	virtual bool reinitWorkingCopy(QString const &targetProject = QString()) = 0;

	/// Returns information about versioning of specified project.
	/// The result format is specific for each VCS and it can return
	/// info about different details.
	/// Examples: svn info
	virtual QString information(QString const &targetProject = QString()) = 0;

	/// Returns the order number of current working copy revision
	virtual QString commitId(QString const &targetProject = QString()) = 0;

	/// Returns the address of the remote repository
	virtual QString remoteRepositoryUrl(QString const &targetProject = QString()) = 0;

	/// Implementation must determine if specified working copy
	/// is versioned with this VCS.
	/// @param directory Path to interested directory
	virtual bool isMyWorkingCopy(
		QString const &directory = QString()
		, bool quiet = false
		, bool prepareAndProcess = false
	) = 0;

	///This method return name of plugin. Examples: GitPlugin..
	virtual QString friendlyName() = 0;

	///This method return true if vcs plugin have installed client
	virtual bool clientExist() = 0;

signals:
	/// Emitted when working copy downloading started by
	/// @see beginWorkingCopyDownloading() complete
	/// @param success Was operation successfull or not
	/// @param targetProject A path to *.qrs-project saved as result of downloading
	void workingCopyDownloaded(bool const success, QString const targetProject);

	/// Emitted when working copy updating started by
	/// @see beginWorkingCopyUpdating() complete
	/// @param success Was operation successfull or not
	void workingCopyUpdated(const bool success);

	/// Emitted when working copy changes submitting started by
	/// @see beginChangesSubmitting() complete
	/// @ param success Was operation successfull or not
	void changesSubmitted(const bool success);

	/// Emitted when in preferences plugins page changed pass to client.
	void clientInstalled(QString client, bool exist);
};

}
