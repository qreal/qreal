#pragma once

#include <functional>

#include <qrkernel/ids.h>

#include "qrtext/debuggerInterface.h"
#include "qrtext/core/error.h"
#include "qrtext/core/ast/node.h"
#include "qrtext/core/types/typeExpression.h"

namespace qrtext {

/// Interface for language toolboxes (toolbox is a facade that integrates lexer, parser, semantic analyzer and
/// interpreter, and provides convenient methods to use them).
class LanguageToolboxInterface : public DebuggerInterface
{
public:
	virtual ~LanguageToolboxInterface() {}

	/// Interprets given code string using given id and property name for connection. Returns result of an expression.
	template<typename T>
	T interpret(qReal::Id const &id, QString const &propertyName, QString const &code)
	{
		auto root = parse(id, propertyName, code);
		if (errors().isEmpty()) {
			return interpret(root).value<T>();
		} else {
			return {};
		}
	}

	/// Interprets given code string without connection. Returns result of an expression.
	template<typename T>
	T interpret(QString const &code)
	{
		return interpret<T>(qReal::Id(), "", code);
	}

	/// Interprets given code string using given id and property name for connection.
	virtual void interpret(qReal::Id const &id, QString const &propertyName, QString const &code) = 0;

	/// Interprets given code string without connection.
	virtual void interpret(QString const &code) = 0;

	/// Parses given code string using given id and property name for connection, returns AST.
	virtual QSharedPointer<core::ast::Node> const &parse(qReal::Id const &id
			, QString const &propertyName
			, QString const &code) = 0;

	/// Returns previously parsed AST for given node and property, or null if no such AST was parsed before.
	virtual QSharedPointer<core::ast::Node> ast(qReal::Id const &id, QString const &propertyName) const = 0;

	/// Returns type of given AST node.
	virtual QSharedPointer<core::types::TypeExpression> type(
			QSharedPointer<core::ast::Node> const &expression) const = 0;

	/// Returns list of errors that were reported during parsing or interpretation.
	virtual QList<core::Error> const &errors() const = 0;

	/// Register intrinsic function.
	/// @param name - name of a function.
	/// @param returnType - function return type, as type expression. Takes ownership.
	/// @param parameterTypes - a list of types of function parameters. Takes ownership.
	/// @param semantic - a function that will be called by interpreter to actually get a result.
	virtual void addIntrinsicFunction(QString const &name
			, core::types::TypeExpression * const returnType
			, const QList<core::types::TypeExpression *> &parameterTypes
			, std::function<QVariant(QList<QVariant> const &)> const &semantic) = 0;

	/// Returns a mapping of variable identifiers to their types.
	virtual QMap<QString, QSharedPointer<core::types::TypeExpression>> variableTypes() const = 0;

private:
	/// Interprets given AST. Returns result of an expression. Must be implemented for concrete language.
	virtual QVariant interpret(QSharedPointer<core::ast::Node> const &root) = 0;
};

}

