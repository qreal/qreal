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

	CheckStatus(bool const &checkStatus, QString const &message, ErrorType const &errorType);
	bool checkStatus() const;
	ErrorType errorType() const;
	QString message() const;

	bool operator==(CheckStatus const &element);

	static QList<CheckStatus> resultCheckStatusList(QList<CheckStatus> const& listCheckStatus);
	static QList<CheckStatus> defaultCheckStatusAsList();

private:
	bool mCheckStatus;
	QString mMessage;
	ErrorType mErrorType;
};

}
