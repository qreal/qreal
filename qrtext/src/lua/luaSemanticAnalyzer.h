/* Copyright 2007-2015 QReal Research Group
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

#include <QtCore/QScopedPointer>

#include "qrtext/core/semantics/semanticAnalyzer.h"
#include "qrtext/lua/types/function.h"

#include "qrtext/declSpec.h"

namespace qrtext {
namespace lua {
namespace details {

/// Semantic analyzer for Lua language.
class QRTEXT_EXPORT LuaSemanticAnalyzer : public core::SemanticAnalyzer
{
public:
	/// Constructor.
	/// @param errors - error stream to report errors to.
	explicit LuaSemanticAnalyzer(QList<core::Error> &errors);

	/// Registers external (intrinsic for a language) function in interpreter.
	/// @param name - name of a function.
	/// @param type - type of a function.
	void addIntrinsicFunction(const QString &name, const QSharedPointer<types::Function> &type);

	/// Registers variable with given name as read-only (or predefined).
	void addReadOnlyVariable(const QString &name);

	/// Override that excludes intrinsic function identifiers.
	QMap<QString, QSharedPointer<core::types::TypeExpression>> variableTypes() const;

	void clear() override;

private:
	void precheck(QSharedPointer<core::ast::Node> const &node) override;
	void analyzeNode(const QSharedPointer<core::ast::Node> &node) override;

	void analyzeUnaryOperator(const QSharedPointer<core::ast::Node> &node);
	void analyzeBinaryOperator(const QSharedPointer<core::ast::Node> &node);

	void constrainAssignment(const QSharedPointer<core::ast::Node> &operation
			, const QSharedPointer<core::ast::Node> &lhs, const QSharedPointer<core::ast::Node> &rhs);

	void analyzeFunctionCall(const QSharedPointer<core::ast::Node> &node);

	/// Reports errors about all identifiers first declared in a given subtree. Supposed to be called to check
	/// right-hand side of an assignment, where shall not be any declarations.
	/// It looks hacky, but when we analyze assignment we've already visited identifiers inside, but had no information
	/// about side of assignment operator there (SemanticAnalyser knows nothing about assignment, so can not pass
	/// such information down to recursive tree traversal).
	void checkForUndeclaredIdentifiers(const QSharedPointer<core::ast::Node> &node);

	/// Reports errors about all read-only variables. Supposed to be called on left side of assignment.
	/// @returns true, if everything is ok.
	bool checkForReadOnlyVariables(const QSharedPointer<core::ast::Node> &node);

	/// Checks that intrinsic functions are used as functions and not as some identifiers.
	void checkReservedIdentifiersUsage(const QSharedPointer<core::ast::Node> &node
			, const QSharedPointer<core::ast::Node> &parent);

	QSharedPointer<core::types::TypeExpression> mBoolean;
	QSharedPointer<core::types::TypeExpression> mFloat;
	QSharedPointer<core::types::TypeExpression> mInteger;
	QSharedPointer<core::types::TypeExpression> mNil;
	QSharedPointer<core::types::TypeExpression> mString;

	QHash<QString, QSharedPointer<types::Function>> mIntrinsicFunctions;

	/// A set of predefined variables.
	QSet<QString> mReadOnlyVariables;
};

}
}
}
