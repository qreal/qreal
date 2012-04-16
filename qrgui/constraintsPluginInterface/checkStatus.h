#pragma once

#include <QtCore/QString>

namespace qReal {

class CheckStatus {
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

	static CheckStatus resultCheckStatus(QList<CheckStatus> const& listCheckStatus); //qwerty

private:
	bool mCheckStatus;
	QString mMessage;
	ErrorType mErrorType;
};

}
