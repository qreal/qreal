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

#include <qrutils/versioningUtils/briefVersioningInterface.h>
#include <qrrepo/workingCopyInspectionInterface.h>
#include <qrrepo/workingCopyManagementInterface.h>

#include "qrgui/versioning/diffPluginInterface.h"

#include "versioningPluginsManagerDeclSpec.h"


namespace qReal {

/// Base class for all VCS cliens. Consists of 3 parts: view part,
/// repo part and brief interface. View part customizes UI with
/// actions and settings tabs. Repo part works with qReal repository
/// component to maintain working copy in correct state.
/// Brief interface provides a number of actions that each VCS
/// should be able to invoke by definition.
class QRUTILS_EXPORT VersioningPluginInterface
		: public BriefVersioningInterface
		, public ToolPluginInterface
		, public qrRepo::WorkingCopyInspectionInterface
{
public:
	virtual ~VersioningPluginInterface() {}

	virtual void setWorkingCopyManager(
		qrRepo::WorkingCopyManagementInterface *workingCopyManager
	) = 0;

	virtual void setDiffViewerInterface(DiffPluginInterface *diffInterface) = 0;
};

}
