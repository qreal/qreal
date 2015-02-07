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
	void addIntrinsicFunction(const QString &name
			, std::function<QVariant(const QList<QVariant> &)> const &semantic);

	/// Interprets given AST using type information provided by given semantic analyzer, returns the result of
	/// calculation or QVariant() if there is no result (error or AST is not supposed to return anything).
	/// @todo Remove direct reference to semanticAnalyzer.
	QVariant interpret(const QSharedPointer<core::ast::Node> &root, const core::SemanticAnalyzer &semanticAnalyzer);

	/// Returns a list of known identifiers
	QStringList identifiers() const;

	/// Returns a value of an identifier with given name.
	QVariant value(const QString &identifier) const;

	/// Sets a value of identifier with given name to given value.
	void setVariableValue(const QString &name, const QVariant &value);

	/// Clear all execution state, except added intrinsic functions.
	void clear();

private:
	QVariant interpretUnaryOperator(const QSharedPointer<core::ast::Node> &root
			, const core::SemanticAnalyzer &semanticAnalyzer);

	QVariant interpretBinaryOperator(const QSharedPointer<core::ast::Node> &root
			, const core::SemanticAnalyzer &semanticAnalyzer);

	QVariant operateOnIndexingExpression(const QSharedPointer<core::ast::Node> &indexingExpression
			, const core::SemanticAnalyzer &semanticAnalyzer
			, std::function<QVariant(const QString &, QStringList &, int)> const &action);

	QHash<QString, QVariant> mIdentifierValues;
	QHash<QString, std::function<QVariant(const QList<QVariant> &)>> mIntrinsicFunctions;

	QList<core::Error> &mErrors;
};

}
}
}
