#include "qrtext/lua/luaToolbox.h"

#include "qrtext/src/lua/luaLexer.h"
#include "qrtext/src/lua/luaParser.h"
#include "qrtext/src/lua/luaSemanticAnalyzer.h"
#include "qrtext/src/lua/luaInterpreter.h"

using namespace qrtext::lua;
using namespace qrtext::core;
using namespace qrtext::core::ast;

LuaToolbox::LuaToolbox()
	: mLexer(new details::LuaLexer(mErrors))
	, mParser(new details::LuaParser(mErrors))
	, mAnalyzer(new details::LuaSemanticAnalyzer(mErrors))
	, mInterpreter(new details::LuaInterpreter())
{
}

LuaToolbox::~LuaToolbox()
{
}

QVariant LuaToolbox::interpret(QSharedPointer<Node> const &root)
{
	return mInterpreter->interpret(root, *mAnalyzer);
}

QSharedPointer<Node> const &LuaToolbox::parse(qReal::Id const &id, QString const &propertyName
		, QString const &code)
{
	mErrors.clear();

	auto tokenStream = mLexer->tokenize(code);
	auto ast = mParser->parse(tokenStream);
	mAstRoots[id][propertyName] = ast;

	mAnalyzer->analyze(ast);

	return mAstRoots[id][propertyName];
}

QSharedPointer<Node> LuaToolbox::ast(qReal::Id const &id, QString const &propertyName) const
{
	return mAstRoots[id][propertyName];
}

QSharedPointer<qrtext::core::types::TypeExpression> LuaToolbox::type(QSharedPointer<Node> const &expression) const
{
	return mAnalyzer->type(expression);
}

QList<Error> const &LuaToolbox::errors() const
{
	return mErrors;
}
