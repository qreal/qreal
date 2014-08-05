#include "textLanguageParser/parserError.h"

using namespace textLanguageParser;

ParserError::ParserError(ast::Connection const &connection, QString const &errorMessage
		, ErrorType errorType, Severity severity)
	: mConnection(connection), mErrorMessage(errorMessage), mErrorType(errorType), mSeverity(severity)
{
}

ast::Connection const &ParserError::connection() const
{
	return mConnection;
}

QString const &ParserError::errorMessage() const
{
	return mErrorMessage;
}

ErrorType ParserError::errorType() const
{
	return mErrorType;
}

Severity ParserError::severity() const
{
	return mSeverity;
}
