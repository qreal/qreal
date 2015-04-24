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

/// Used for notification about working copy modification.
class WorkingCopyInspectionInterface
{
public:
	virtual ~WorkingCopyInspectionInterface() {}

	/// Called when file was added to working copy and
	/// needs to be included under version control
	virtual bool onFileAdded(const QList<QString> &list, QString const &workingDir) = 0;

	/// Called when file was removed from working copy and
	/// needs to be excluded from version control
	virtual bool onFileRemoved(const QList<QString> &list, QString const &workingDir) = 0;

	/// Called when file in working copy was changed
	virtual bool onFileChanged(const QList<QString> &list, QString const &workingDir) = 0;
};

}
