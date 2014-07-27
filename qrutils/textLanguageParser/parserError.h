#pragma once

#include <QtCore/QString>

#include "textLanguageParser/ast/connection.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {

class ParserError {
public:
	enum ErrorType {
		lexicalError
		, syntaxError
		, semanticError
	};

	enum Severity {
		internalError
		, warning
		, error
		, critical
	};

	ParserError(ast::Connection const &connection, QString const &errorMessage, ErrorType errorType, Severity severity);

	ast::Connection const &connection() const;
	QString const &errorMessage() const;
	ErrorType errorType() const;
	Severity severity() const;

private:
	ast::Connection mConnection;
	QString mErrorMessage;
	ErrorType mErrorType;
	Severity mSeverity;
};

}
