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

CheckStatus CheckStatus::resultCheckStatus(QList<CheckStatus> const& listCheckStatus)
{
	if (listCheckStatus.empty()) {
		return CheckStatus(true, "none", qReal::CheckStatus::warning);//qwerty : ошибка : скорее всего неправильное имя языка задано для элемента
	}
	foreach (CheckStatus const checkStatus, listCheckStatus) {
		if (checkStatus.checkStatus() == false) {
			return checkStatus;
		}
	}
	return listCheckStatus.at(0);
}
