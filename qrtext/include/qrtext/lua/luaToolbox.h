#pragma once

#include <functional>

#include <QtCore/QSharedPointer>
#include <QtCore/QScopedPointer>

#include <qrkernel/ids.h>

#include "qrtext/core/ast/node.h"
#include "qrtext/core/types/typeExpression.h"
#include "qrtext/core/error.h"
#include "qrtext/core/lexer/tokenPatterns.h"

#include "qrtext/core/types/typeExpression.h"

#include "qrtext/declSpec.h"

namespace qrtext {
namespace lua {

namespace details {
class LuaLexer;
class LuaParser;
class LuaSemanticAnalyzer;
class LuaInterpreter;
}

typedef core::Error Error;

/// Facade that uses text language parsing/interpreting framework to provide services based on "QReal standard" text
/// language (Lua 5.3 subset).
///
/// Facade can be used for interpreting diagrams or for syntax analysis in generators. It allows to pass code fragments
/// as chunks (corresponding to Lua blocks) identified by Id of a node and name of the property where this code fragment
/// was taken from. It can interpret it immediately, returning result and storing syntax tree and interpreter state for
/// future uses, or can parse it and then return AST for node/property by request (note that it uses all ASTs
/// as a context for parsing next chunks).
///
/// Note that types of variables may change during parsing next chunks, so, for example, after parsing "a = 123" type
/// of "a" will be inferred as Integer, but after parsing "a = 1.0" it will be changed to Float. Generators may reliably
/// use type information only when all code in a program is parsed.
class QRTEXT_EXPORT LuaToolbox {
public:
	LuaToolbox();
	~LuaToolbox();

	template<typename T>
	T interpret(qReal::Id const &id, QString const &propertyName, QString const &code)
	{
		auto root = parse(id, propertyName, code);
		return interpret(root).value<T>();
	}

	template<typename T>
	T interpret(QString const &code)
	{
		auto root = parse(qReal::Id(), "", code);
		return interpret(root).value<T>();
	}

	void interpret(qReal::Id const &id, QString const &propertyName, QString const &code)
	{
		interpret<int>(id, propertyName, code);
	}

	void interpret(QString const &code)
	{
		interpret<int>(qReal::Id(), "", code);
	}

	void markAsChanged(qReal::Id const &id, QString const &propertyName);

	QSharedPointer<core::ast::Node> const &parse(qReal::Id const &id, QString const &propertyName, QString const &code);

	QSharedPointer<core::ast::Node> ast(qReal::Id const &id, QString const &propertyName) const;

	QSharedPointer<core::types::TypeExpression> type(QSharedPointer<core::ast::Node> const &expression) const;

	QList<core::Error> const &errors() const;

	/// Register intrinsic function.
	/// @param name - name of a function.
	/// @param returnType - function return type, as type expression. Takes ownership.
	/// @param parameterTypes - a list of types of function parameters. Takes ownership.
	/// @param semantic - a function that will be called by interpreter to actually get a result.
	void addIntrinsicFunction(QString const &name
			, core::types::TypeExpression * const returnType
			, const QList<core::types::TypeExpression *> &parameterTypes
			, std::function<QVariant(QList<QVariant> const &)> const &semantic);

	QStringList identifiers() const;

	template<typename T>
	T value(QString const &identifier) const
	{
		return value(identifier).value<T>();
	}

private:
	QVariant interpret(QSharedPointer<core::ast::Node> const &root);
	QVariant value(QString const &identifier) const;

	QList<core::Error> mErrors;

	QScopedPointer<details::LuaLexer> mLexer;
	QScopedPointer<details::LuaParser> mParser;
	QScopedPointer<details::LuaSemanticAnalyzer> mAnalyzer;
	QScopedPointer<details::LuaInterpreter> mInterpreter;

	QHash<qReal::Id, QHash<QString, QSharedPointer<core::ast::Node>>> mAstRoots;
};

}
}
