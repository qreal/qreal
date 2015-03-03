#include "generatorBase/lua/luaProcessor.h"

#include <qrtext/languageToolboxInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "src/lua/luaPrinter.h"

using namespace generatorBase::lua;
using namespace qReal;

LuaProcessor::LuaProcessor(qReal::ErrorReporterInterface &errorReporter
		, qrtext::LanguageToolboxInterface &textLanguage
		, const utils::ParserErrorReporter &parserErrorReporter
		, QObject *parent)
	: QObject(parent)
	, TemplateParametrizedEntity()
	, mErrorReporter(errorReporter)
	, mTextLanguage(textLanguage)
	, mParserErrorReporter(parserErrorReporter)
{
}

QString LuaProcessor::translate(const QString &data
		, const Id &id
		, const QString &propertyName
		, const simple::Binding::ConverterInterface *reservedVariablesConverter)
{
	const QSharedPointer<qrtext::core::ast::Node> tree = parse(data, id, propertyName);
	return lua::LuaPrinter(pathToRoot(), mTextLanguage
			, precedenceConverter(), reservedVariablesConverter).print(tree);
}


QString LuaProcessor::castToString(const QString &data
		, const Id &id
		, const QString &propertyName
		, const simple::Binding::ConverterInterface *reservedVariablesConverter)
{
	const QSharedPointer<qrtext::core::ast::Node> tree = parse(data, id, propertyName);
	return lua::LuaPrinter(pathToRoot(), mTextLanguage
			, precedenceConverter(), reservedVariablesConverter).castToString(tree);
}

QSharedPointer<qrtext::core::ast::Node> LuaProcessor::parse(const QString &data
		, const qReal::Id &id
		, const QString &propertyName) const
{
	const QSharedPointer<qrtext::core::ast::Node> tree = mTextLanguage.parse(id, propertyName, data);
	if (!mTextLanguage.errors().isEmpty()) {
		mParserErrorReporter.reportErrors(id, propertyName);
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
