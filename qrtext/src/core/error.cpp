#include "qrtext/core/error.h"

using namespace qrtext::core;

Error::Error(const Connection &connection, const QString &errorMessage
		, ErrorType errorType, Severity severity)
	: mConnection(connection), mErrorMessage(errorMessage), mErrorType(errorType), mSeverity(severity)
{
}

const Connection &Error::connection() const
{
	return mConnection;
}

const QString &Error::errorMessage() const
{
	return mErrorMessage;
}

ErrorType Error::errorType() const
{
	return mErrorType;
}

Severity Error::severity() const
{
	return mSeverity;
}
