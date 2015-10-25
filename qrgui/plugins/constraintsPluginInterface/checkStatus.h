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
#include "constraintsPluginInterfaceDeclSpec.h"

namespace qReal {

class QRGUI_CONSTRAINTS_PLUGIN_INTERFACE_EXPORT CheckStatus
{
public:
	enum ErrorType {
		warning,
		critical,
		verification
	};

	/// Constructor for class CheckStatus.
	/// @param checkStatus - check status.
	/// @param message - error message.
	/// @param errorType - type of error.
	CheckStatus(const bool &checkStatus
			, const QString &message
			, const ErrorType &errorType);

	/// Returns check status.
	bool checkStatus() const;

	/// Returns check status error type.
	ErrorType errorType() const;

	/// Returns check status message.
	QString message() const;

	/// Returns true if given check status is equal to element.
	/// @param element - other check status.
	bool operator==(CheckStatus const &element);

	/// Returns list of check statuses without duplication.
	/// @param listCheckStatus - check status list.
	static QList<CheckStatus> resultCheckStatusList(const QList<CheckStatus> &listCheckStatus);

	/// Returns empty check status list.
	static QList<CheckStatus> defaultCheckStatusAsList();

private:
	bool mCheckStatus;
	QString mMessage;
	ErrorType mErrorType;
};

}
