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
	for (qrtext::core::Error const &error : mParser.errors()) {
		QString const errorMessage = QString("'%1', %2:%3 %4")
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
