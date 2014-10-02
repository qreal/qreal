#include "luaProcessor.h"

#include <qrtext/languageToolboxInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "src/lua/luaPrinter.h"

using namespace generatorBase::lua;
using namespace qReal;

LuaProcessor::LuaProcessor(qReal::ErrorReporterInterface &errorReporter
		, qrtext::LanguageToolboxInterface &textLanguage
		, QObject *parent)
	: QObject(parent)
	, TemplateParametrizedEntity()
	, mErrorReporter(errorReporter)
	, mTextLanguage(textLanguage)
{
}

QString LuaProcessor::translate(QString const &data
		, Id const &id
		, simple::Binding::ConverterInterface const *reservedVariablesConverter)
{
	QSharedPointer<qrtext::core::ast::Node> const tree = mTextLanguage.parse(id, QString(), data);
	if (!mTextLanguage.errors().isEmpty()) {
		/// @todo: move this code to some common place
		for (qrtext::core::Error const &error : mTextLanguage.errors()) {
			switch (error.severity()) {
			case qrtext::core::Severity::error:
				mErrorReporter.addError(error.errorMessage(), error.connection().id());
				break;
			case qrtext::core::Severity::critical:
				mErrorReporter.addCritical(error.errorMessage(), error.connection().id());
				break;
			case qrtext::core::Severity::warning:
				mErrorReporter.addWarning(error.errorMessage(), error.connection().id());
				break;
			default:
				break;
			}
		}

		return QString();
	}

	return lua::LuaPrinter(pathToRoot(), precedenceConverter(), reservedVariablesConverter).print(tree);
}

PrecedenceConverterInterface &LuaProcessor::precedenceConverter()
{
	return mPrecedenceConverter;
}
