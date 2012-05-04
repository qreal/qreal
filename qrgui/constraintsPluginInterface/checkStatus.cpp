#include "checkStatus.h"
#include <QtCore/QList>

using namespace qReal;

CheckStatus::CheckStatus(bool const &checkStatus, QString const &message, ErrorType const &errorType)
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

QList<CheckStatus> CheckStatus::resultCheckStatusList(QList<CheckStatus> const& listCheckStatus)
{
	QList<CheckStatus> resCheckStatusList = QList<CheckStatus>();
	foreach (CheckStatus const& checkStatus, listCheckStatus) {
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
	if (mCheckStatus == element.checkStatus() && mMessage == element.message() && mErrorType == element.errorType()) {
		return true;
	}
	return false;
}
