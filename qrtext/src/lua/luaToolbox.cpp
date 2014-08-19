#include "qrtext/lua/luaToolbox.h"

using namespace qrtext::lua;
using namespace qrtext::core;
using namespace qrtext::core::lexer;

LuaToolbox::LuaToolbox()
	: mLexer(mErrors), mParser(mErrors)
{
}

QSharedPointer<ast::Node> const &LuaToolbox::parse(qReal::Id const &id, QString const &propertyName
		, QString const &code)
{
	mErrors.clear();

	auto tokenStream = mLexer.tokenize(code);
	auto ast = mParser.parse(tokenStream);
	mAstRoots[id][propertyName] = ast;

//	mAnalyzer.analyze(result.astRoot);

	return mAstRoots[id][propertyName];
}

QSharedPointer<ast::Node> LuaToolbox::ast(qReal::Id const &id, QString const &propertyName) const
{
	return mAstRoots[id][propertyName];
}

//	QSharedPointer<types::TypeExpression> type(QSharedPointer<ast::Node> const &expression) const;

QList<Error> const &LuaToolbox::errors() const
{
	return mErrors;
}
