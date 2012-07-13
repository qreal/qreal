#pragma once

#include "../pluginManager/toolPluginManager.h"
#include "briefVersioningInterface.h"
#include "../../qrrepo/workingCopyInspectionInterface.h"
#include "../../qrrepo/workingCopyManagementInterface.h"

namespace qReal
{
namespace versioning
{

/// Base class for all VCS cliens. Consists of 3 parts: view part,
/// repo part and brief interface. View part customizes UI with
/// actions and settings tabs. Repo part works with qReal repository
/// component to maintain working copy in correct state.
/// Brief interface provides a number of actions that each VCS
/// should be able to invoke by definition.
class VersioningPluginInterface :
		public ToolPluginInterface,
		public BriefVersioningInterface,
		public qrRepo::versioning::WorkingCopyInspectionInterface
{
public:
	virtual ~VersioningPluginInterface() {}

	virtual void setWorkingCopyManager(
			qrRepo::versioning::WorkingCopyManagementInterface *workingCopyManager) = 0;
};

}
}
