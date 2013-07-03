#pragma once

#include <QtCore/QString>

namespace qrRepo
{

/// Provides a number of methods needed for VCS client
/// from repository component for correct versioning
class WorkingCopyManagementInterface
{
public:
	virtual ~WorkingCopyManagementInterface() {}

	/// Called before VCS operation; implementation must
	/// guarantee the working copy existance in target folder
	/// after this method is invoked
	/// @param targetFolder A path to folder in that working copy must appear
	/// @param sourceProject A path to *.qrs-project
	virtual void prepareWorkingCopy(QString const &targetFolder, QString const &sourceProject = QString()) = 0;

	/// Called after VCS operation; implementation must
	/// process changes in working copy so that system will
	/// know about them
	/// @param workinCopyPath A path to working copy
	/// @param sourceProject A path to *.qrs-project that must obtain chenges in working copy
	virtual void processWorkingCopy(QString const &workingCopyPath, QString const &targetProject = QString()) = 0;
};

}
