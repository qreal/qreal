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

#include "parserErrorReporter.h"

#include <qrtext/core/error.h>
#include <qrtext/languageToolboxInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

using namespace utils;

ParserErrorReporter::ParserErrorReporter(const qrtext::LanguageToolboxInterface &parser
		, qReal::ErrorReporterInterface &errorReporter
		, const qReal::EditorManagerInterface &editorManagerInterface)
	: mParser(parser)
	, mErrorReporter(errorReporter)
	, mEditorManagerInterface(editorManagerInterface)
{
}

void ParserErrorReporter::reportErrors(const qReal::Id &id, const QString &property) const
{
	for (const qrtext::core::Error &error : mParser.errors()) {
		const QString errorMessage = QString("'%1', %2:%3 %4")
				.arg(mEditorManagerInterface.propertyDisplayedName(id, property))
				.arg(error.connection().line() + 1)
				.arg(error.connection().column() + 1)
				.arg(error.errorMessage());

		switch (error.severity()) {
		case qrtext::core::Severity::critical:
		case qrtext::core::Severity::error:
			mErrorReporter.addError(errorMessage, id);

			break;
		case qrtext::core::Severity::warning:
			mErrorReporter.addWarning(errorMessage, id);

			break;
		case qrtext::core::Severity::internalError:
			break;
		}
	}
}
