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

#pragma once

#include <QtCore/QString>

#include "qrtext/core/connection.h"

#include "qrtext/declSpec.h"

namespace qrtext {
namespace core {

/// Component that detected an error.
enum class ErrorType
{
	lexicalError
	, syntaxError
	, semanticError
	, runtimeError
};

/// Severity of an error. Internal error means that there is a coding error somewhere in text language implementation
/// and is not supposed to be shown to user.
enum class Severity
{
	internalError
	, warning
	, error
	, critical
};

/// Represents various errors in text language processing module.
class QRTEXT_EXPORT Error
{
public:
	/// Constructor.
	/// @param connection - point in code where an error was detected.
	/// @param errorMessage - textual description of an error.
	/// @param errorType - what component (lexer/parser/so on) reported error.
	/// @param severity - severity of an error.
	Error(const Connection &connection, const QString &errorMessage, ErrorType errorType, Severity severity);

	/// Returns point in code where an error was detected.
	const Connection &connection() const;

	/// Returns textual description of an error.
	const QString &errorMessage() const;

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
