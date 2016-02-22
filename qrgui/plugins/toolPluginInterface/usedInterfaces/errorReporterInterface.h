/* Copyright 2007-2016 QReal Research Group
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

#include <qrkernel/ids.h>

#include "qrgui/plugins/toolPluginInterface/usedInterfaces/progressReporterInterface.h"

namespace qReal {

class ErrorReporterInterface : public ProgressReporterInterface
{
public:
	virtual ~ErrorReporterInterface() {}
	virtual void addInformation(const QString &message, const Id &position = Id::rootId()) = 0;
	virtual void addWarning(const QString &message, const Id &position = Id::rootId()) = 0;
	virtual void addError(const QString &message, const Id &position = Id::rootId()) = 0;
	virtual void addCritical(const QString &message, const Id &position = Id::rootId()) = 0;

	/// Shows beautifully bubbling \a message in the top-right corner of \a parent for
	/// \a duration / 2 milliseconds, then the message fades away for the same time.
	/// @param parent The widget in top-right corner of which the message is shown.
	/// If nullptr is passed then main window will be used.
	virtual void sendBubblingMessage(const QString &message, int duration, QWidget *parent = 0) = 0;

	virtual void clear() = 0;
	virtual void clearErrors() = 0;

	/// Are there errors reported by now or not. Counted after latest clearErrors() call
	/// @returns True, when there are messages with severity "Critical" and "Error" registered
	virtual bool wereErrors() = 0;
};

}
