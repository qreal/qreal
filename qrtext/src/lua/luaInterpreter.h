/* Copyright 2007-2015 QReal Research Group, Yurii Litvinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <functional>
#include <QtCore/QHash>
#include <QtCore/QVariantList>

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

	/// Registers variable with given name as read-only (can be modified only by setVariableValue() call).
	void addReadOnlyVariable(const QString &name);

	/// Clear all execution state, except added intrinsic functions.
	void clear();

private:
	QVariant interpretUnaryOperator(const QSharedPointer<core::ast::Node> &root
			, const core::SemanticAnalyzer &semanticAnalyzer);

	QVariant interpretBinaryOperator(const QSharedPointer<core::ast::Node> &root
			, const core::SemanticAnalyzer &semanticAnalyzer);

	QVariant operateOnIndexingExpression(const QSharedPointer<core::ast::Node> &indexingExpression
			, const core::SemanticAnalyzer &semanticAnalyzer
			, const std::function<QVariant(const QString &
					, const QVariantList &
					, const QVector<int> &
					, const core::Connection &)> &action);

	QVariant constructTable(const QSharedPointer<core::ast::Node> &tableConstructor
			, const core::SemanticAnalyzer &semanticAnalyzer);

	void assignToTableElement(const QSharedPointer<core::ast::Node> &variable, const QVariant &interpretedValue
			, const core::SemanticAnalyzer &semanticAnalyzer);

	QVariantList doAssignToTableElement(const QVariantList &table
			, const QVariant &value
			, const QVector<int> &index
			, const core::Connection &connection);

	QVariant slice(const QSharedPointer<core::ast::Node> &indexingExpression
			, const core::SemanticAnalyzer &semanticAnalyzer);

	QVariant operateOnIndexingExpressionRecursive(const QSharedPointer<core::ast::Node> &indexingExpression
			, const QVector<int> &currentIndex, const core::SemanticAnalyzer &semanticAnalyzer
			, const std::function<QVariant(const QString &
					, const QVariantList &
					, const QVector<int> &
					, const core::Connection &)> &action);

	QHash<QString, QVariant> mIdentifierValues;
	QHash<QString, std::function<QVariant(const QList<QVariant> &)>> mIntrinsicFunctions;

	/// A set of variables which can be modified only by setVariableValue() call (used to support sensor variables and
	/// ailases)
	QSet<QString> mReadOnlyVariables;

	QList<core::Error> &mErrors;
};

}
}
}
