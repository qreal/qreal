#include "qrtext/lua/luaToolbox.h"

using namespace qrtext::lua;
using namespace qrtext::core;
using namespace qrtext::core::ast;
using namespace qrtext::core::lexer;

LuaToolbox::LuaToolbox()
	: mLexer(mErrors), mParser(mErrors), mAnalyzer(mErrors)
{
}

QSharedPointer<Node> const &LuaToolbox::parse(qReal::Id const &id, QString const &propertyName
		, QString const &code)
{
	mErrors.clear();

	auto tokenStream = mLexer.tokenize(code);
	auto ast = mParser.parse(tokenStream);
	mAstRoots[id][propertyName] = ast;

	mAnalyzer.analyze(ast);

	return mAstRoots[id][propertyName];
}

QSharedPointer<Node> LuaToolbox::ast(qReal::Id const &id, QString const &propertyName) const
{
	return mAstRoots[id][propertyName];
}

QSharedPointer<types::TypeExpression> LuaToolbox::type(QSharedPointer<Node> const &expression) const
{
	return mAnalyzer.type(expression);
}

QList<Error> const &LuaToolbox::errors() const
{
	return mErrors;
}
