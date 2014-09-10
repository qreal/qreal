#pragma once

#include <QtCore/QString>

#include "qrtext/core/connection.h"

#include "qrtext/declSpec.h"

namespace qrtext {
namespace core {

/// Component that detected an error.
enum class ErrorType {
	lexicalError
	, syntaxError
	, semanticError
	, runtimeError
};

/// Severity of an error. Internal error means that there is a coding error somewhere in text language implementation
/// and is not supposed to be shown to user.
enum class Severity {
	internalError
	, warning
	, error
	, critical
};

/// Represents various errors in text language processing module.
class QRTEXT_EXPORT Error {
public:
	/// Constructor.
	/// @param connection - point in code where an error was detected.
	/// @param errorMessage - textual description of an error.
	/// @param errorType - what component (lexer/parser/so on) reported error.
	/// @param severity - severity of an error.
	Error(Connection const &connection, QString const &errorMessage, ErrorType errorType, Severity severity);

	/// Returns point in code where an error was detected.
	Connection const &connection() const;

	/// Returns textual description of an error.
	QString const &errorMessage() const;

	/// Returns what component (lexer/parser/so on) reported error.
	ErrorType errorType() const;

	/// Returns severity of an error.
	Severity severity() const;

private:
	Connection mConnection;
	QString mErrorMessage;
	ErrorType mErrorType;
	Severity mSeverity;
};

}
}
