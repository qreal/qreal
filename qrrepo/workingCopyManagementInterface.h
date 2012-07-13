#pragma once

#include <QString>

namespace qrRepo
{
namespace versioning
{

/// Provides a number of methods needed for VCS client
/// from repository component for correct versioning
class WorkingCopyManagementInterface
{
public:
	virtual ~WorkingCopyManagementInterface() {}

	/// Called before each VCS operation; implementation must
	/// guarantee the working copy existance in target folder
	/// after this method is invoked
	virtual void prepareWorkingCopy(QString const &workingCopyPath) = 0;

	/// Called after each VCS operation; implementation must
	/// process changes in working copy so that system will
	/// know about them
	virtual void processWorkingCopy(QString const &workingCopyPath, QString const &targetProject = QString()) = 0;
};

}
}
