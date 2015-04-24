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
