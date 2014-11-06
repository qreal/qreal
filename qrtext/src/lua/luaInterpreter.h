#pragma once

#include <functional>
#include <QtCore/QHash>

#include "qrtext/core/error.h"
#include "qrtext/core/ast/node.h"

#include "qrtext/core/semantics/semanticAnalyzer.h"

#include "qrtext/lua/types/function.h"

namespace qrtext {
namespace lua {
namespace details {

/// Interpreter of AST for Lua language.
class LuaInterpreter
{
public:
	/// Constructor.
	/// @param errors - error stream to report errors to.
	explicit LuaInterpreter(QList<core::Error> &errors);

	/// Registers external (intrinsic for a language) function in interpreter.
	/// @param name - name of a function.
	/// @param semantic - actual C++ function that implements it. Must take list of actual parameters and return
	///        calculated value as QVariant.
	void addIntrinsicFunction(QString const &name
			, std::function<QVariant(QList<QVariant> const &)> const &semantic);

	/// Interprets given AST using type information provided by given semantic analyzer, returns the result of
	/// calculation or QVariant() if there is no result (error or AST is not supposed to return anything).
	/// @todo Remove direct reference to semanticAnalyzer.
	QVariant interpret(QSharedPointer<core::ast::Node> const &root, core::SemanticAnalyzer const &semanticAnalyzer);

	/// Returns a list of known identifiers
	QStringList identifiers() const;

	/// Returns a value of an identifier with given name.
	QVariant value(QString const &identifier) const;

	/// Sets a value of identifier with given name to given value.
	void setVariableValue(QString const &name, QVariant const &value);

	/// Clear all execution state, except added intrinsic functions.
	void clear();

private:
	QVariant interpretUnaryOperator(QSharedPointer<core::ast::Node> const &root
			, core::SemanticAnalyzer const &semanticAnalyzer);

	QVariant interpretBinaryOperator(QSharedPointer<core::ast::Node> const &root
			, core::SemanticAnalyzer const &semanticAnalyzer);

	QVariant operateOnIndexingExpression(QSharedPointer<core::ast::Node> const &indexingExpression
			, core::SemanticAnalyzer const &semanticAnalyzer
			, std::function<QVariant(QString const &, QStringList &, int)> const &action);

	QHash<QString, QVariant> mIdentifierValues;
	QHash<QString, std::function<QVariant(QList<QVariant> const &)>> mIntrinsicFunctions;

	QList<core::Error> &mErrors;
};

}
}
}
