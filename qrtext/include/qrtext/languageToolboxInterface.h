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
	T interpret(const qReal::Id &id, const QString &propertyName, const QString &code)
	{
		const auto &root = parse(id, propertyName, code);
		if (errors().isEmpty()) {
			return interpret(root).value<T>();
		} else {
			return {};
		}
	}

	/// Interprets given code string without connection. Returns result of an expression.
	template<typename T>
	T interpret(const QString &code)
	{
		return interpret<T>(qReal::Id(), "", code);
	}

	/// Interprets given code string using given id and property name for connection.
	virtual void interpret(const qReal::Id &id, const QString &propertyName, const QString &code) = 0;

	/// Interprets given code string without connection.
	virtual void interpret(const QString &code) = 0;

	/// Parses given code string using given id and property name for connection, returns AST.
	virtual const QSharedPointer<core::ast::Node> &parse(const qReal::Id &id
			, const QString &propertyName
			, const QString &code) = 0;

	/// Returns previously parsed AST for given node and property, or null if no such AST was parsed before.
	virtual QSharedPointer<core::ast::Node> ast(const qReal::Id &id, const QString &propertyName) const = 0;

	/// Returns type of given AST node.
	virtual QSharedPointer<core::types::TypeExpression> type(
			const QSharedPointer<core::ast::Node> &expression) const = 0;

	/// Returns list of errors that were reported during parsing or interpretation.
	virtual QList<core::Error> const &errors() const = 0;

	/// Register intrinsic function.
	/// @param name - name of a function.
	/// @param returnType - function return type, as type expression. Takes ownership.
	/// @param parameterTypes - a list of types of function parameters. Takes ownership.
	/// @param semantic - a function that will be called by interpreter to actually get a result.
	virtual void addIntrinsicFunction(const QString &name
			, core::types::TypeExpression * const returnType
			, const QList<core::types::TypeExpression *> &parameterTypes
			, std::function<QVariant(const QList<QVariant> &)> const &semantic) = 0;

	/// Returns a mapping of variable identifiers to their types.
	virtual QMap<QString, QSharedPointer<core::types::TypeExpression>> variableTypes() const = 0;

	/// Returns a list of predefined identifiers that are reserved by the system.
	virtual const QStringList &specialIdentifiers() const = 0;

	/// Returns a list of predefined constants that are reserved by the system.
	virtual const QStringList &specialConstants() const = 0;

	/// Clears the state of the parser making it forget types of all identifiers, other expressions and clear
	/// all remembered information except caches.
	virtual void clear() = 0;

	/// Returns true if \a specific type is a subtype of \a general type, including case when they are equivalent.
	virtual bool isGeneralization(const QSharedPointer<core::types::TypeExpression> &specific
			, const QSharedPointer<core::types::TypeExpression> &general) const = 0;

private:
	/// Interprets given AST. Returns result of an expression. Must be implemented for concrete language.
	virtual QVariant interpret(const QSharedPointer<core::ast::Node> &root) = 0;
};

}

