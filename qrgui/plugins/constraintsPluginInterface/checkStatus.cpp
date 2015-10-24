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

#include "checkStatus.h"
#include <QtCore/QList>

using namespace qReal;

CheckStatus::CheckStatus(const bool &checkStatus
		, const QString &message
		, const ErrorType &errorType)
	: mCheckStatus(checkStatus), mMessage(message), mErrorType(errorType)
{
}

bool CheckStatus::checkStatus() const
{
	return mCheckStatus;
}

CheckStatus::ErrorType CheckStatus::errorType() const
{
	return mErrorType;
}

QString CheckStatus::message() const
{
	return mMessage;
}

QList<CheckStatus> CheckStatus::resultCheckStatusList(const QList<CheckStatus> &listCheckStatus)
{
	QList<CheckStatus> resCheckStatusList = defaultCheckStatusAsList();

	for (const CheckStatus &checkStatus : listCheckStatus) {
		if (!resCheckStatusList.contains(checkStatus)) {
			resCheckStatusList.append(checkStatus);
		}
	}

	return resCheckStatusList;
}

QList<CheckStatus> CheckStatus::defaultCheckStatusAsList()
{
	QList<CheckStatus> resList;
	resList.append(CheckStatus(true, "", CheckStatus::warning));

	return resList;
}

bool CheckStatus::operator==(CheckStatus const &element)
{
	return ((mCheckStatus == element.checkStatus()
			&& mMessage == element.message() && mErrorType == element.errorType()));
}
