#include "qrtext/core/error.h"

using namespace qrtext::core;

Error::Error(Connection const &connection, QString const &errorMessage
		, ErrorType errorType, Severity severity)
	: mConnection(connection), mErrorMessage(errorMessage), mErrorType(errorType), mSeverity(severity)
{
}

Connection const &Error::connection() const
{
	return mConnection;
}

QString const &Error::errorMessage() const
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
