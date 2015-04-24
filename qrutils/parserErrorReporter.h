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

#include "qrutils/utilsDeclSpec.h"

namespace qReal {
class Id;
class ErrorReporterInterface;
class EditorManagerInterface;
}

namespace qrtext {
class LanguageToolboxInterface;
}

namespace utils {

/// Utility class to report errors from text language parser.
class QRUTILS_EXPORT ParserErrorReporter
{
public:
	/// Constructor.
	/// @param parser - parser that has error stream we shall look to.
	/// @param errorReporter - Error Reporter object to which we shall put errors from parser.
	ParserErrorReporter(const qrtext::LanguageToolboxInterface &parser
			, qReal::ErrorReporterInterface &errorReporter
			, const qReal::EditorManagerInterface &editorManagerInterface);

	/// Adds errors to Error Reporter according to their severity.
	/// @param id - id of a block which property was parsed.
	/// @param property - name (internal) of a property which was parsed, to provide property name in error report.
	void reportErrors(const qReal::Id &id, const QString &property) const;

private:
	const qrtext::LanguageToolboxInterface &mParser;
	qReal::ErrorReporterInterface &mErrorReporter;
	const qReal::EditorManagerInterface &mEditorManagerInterface;
};

}
