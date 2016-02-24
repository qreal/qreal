/* Copyright 2016 CyberTech Labs Ltd.
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

template<typename TResult>
class QFuture;

namespace qReal {

/// An interface for all components that show to user some internal long activity.
class ProgressReporterInterface
{
public:
	virtual ~ProgressReporterInterface() {}

	/// Will be implemented to show user some computational activity. Long operation should be started in non-GUI
	/// thread not to freeze UI. Recommeded way to start operation is using one of qrutils/runExtensions.h methods.
	/// @param operation QFuture object that provides tracking of operation state.
	/// @param description String description of what operation does, may be shown to user so should be localized.
	virtual void reportOperation(const QFuture<void> &operation, const QString &description = QString()) = 0;
};

}
