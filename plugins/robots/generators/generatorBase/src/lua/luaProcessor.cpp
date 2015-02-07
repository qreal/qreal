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
