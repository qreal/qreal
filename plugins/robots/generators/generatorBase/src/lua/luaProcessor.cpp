#include "generatorBase/lua/luaProcessor.h"

#include <qrtext/languageToolboxInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

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
		, QString const &propertyName
		, simple::Binding::ConverterInterface const *reservedVariablesConverter)
{
	QSharedPointer<qrtext::core::ast::Node> const tree = parse(data, id, propertyName);
	return lua::LuaPrinter(pathToRoot(), mTextLanguage
			, precedenceConverter(), reservedVariablesConverter).print(tree);
}


QString LuaProcessor::castToString(QString const &data
		, Id const &id
		, QString const &propertyName
		, simple::Binding::ConverterInterface const *reservedVariablesConverter)
{
	QSharedPointer<qrtext::core::ast::Node> const tree = parse(data, id, propertyName);
	return lua::LuaPrinter(pathToRoot(), mTextLanguage
			, precedenceConverter(), reservedVariablesConverter).castToString(tree);
}

QSharedPointer<qrtext::core::ast::Node> LuaProcessor::parse(QString const &data
		, qReal::Id const &id
		, QString const &propertyName) const
{
	QSharedPointer<qrtext::core::ast::Node> const tree = mTextLanguage.parse(id, propertyName, data);
	if (!mTextLanguage.errors().isEmpty()) {
		/// @todo: move this code to some common place
		for (qrtext::core::Error const &error : mTextLanguage.errors()) {
			QString const errorMessage = QString("%1:%2 %3")
					.arg(error.connection().line())
					.arg(error.connection().column())
					.arg(error.errorMessage());

			switch (error.severity()) {
			case qrtext::core::Severity::error:
				mErrorReporter.addError(errorMessage, id);
				break;
			case qrtext::core::Severity::critical:
				mErrorReporter.addCritical(errorMessage, id);
				break;
			case qrtext::core::Severity::warning:
				mErrorReporter.addWarning(errorMessage, id);
				break;
			default:
				break;
			}
		}

		return qrtext::wrap(nullptr);
	}

	return tree;
}

qrtext::LanguageToolboxInterface &LuaProcessor::toolbox() const
{
	return mTextLanguage;
}

PrecedenceConverterInterface &LuaProcessor::precedenceConverter()
{
	return mPrecedenceConverter;
}
