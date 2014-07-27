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

ParserError::ErrorType ParserError::errorType() const
{
	return mErrorType;
}

ParserError::Severity ParserError::severity() const
{
	return mSeverity;
}
