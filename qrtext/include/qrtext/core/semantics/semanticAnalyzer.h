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

#include <QtCore/QSharedPointer>
#include <QtCore/QHash>
#include <QtCore/QList>

#include <qrkernel/ids.h>

#include "qrtext/core/error.h"
#include "qrtext/core/ast/node.h"
#include "qrtext/core/ast/expression.h"

#include "qrtext/core/types/typeExpression.h"
#include "qrtext/core/types/typeVariable.h"

#include "qrtext/core/semantics/generalizationsTableInterface.h"

#include "qrtext/declSpec.h"

namespace qrtext {
namespace core {

/// Generic semantic analyzer and type inferrer for languages with implicit static typing, like Lua or JavaScript.
/// Ironically, Lua and JavaScript are dynamically typed, but we can not generate code in a language with static typing
/// (like C) if we don't know the types in compile time, so we use subsets of dynamically typed languages and trying
/// to provide static type systems for them. Of course, also can be used with statically typed languages.
/// Designed and tested for monomorphic type systems (maybe with overloads and coercions), maybe it will work for
/// subtype polymorphism, but correct parametric polymorphism requires full Hindley-Milner implementation which is
/// yet to be written (if needed).
/// If it is not needed, one is free to exclude SemanticAnalyzer from toolchain of one's language.
///
/// Assigns a type to any subclass of Expression in a tree.
/// Shall be subclassed for concrete language to provide methods for analysis of all specific nodes of a language.
///
/// Types of a concrete language are represented by subclasses of TypeExpression class, and can be simple type constants
/// like "int" or "void", or complex types like "function".
///
/// Current implementation only works for simple types.
///
/// Algorithm of type inference is at first assigns to every expression type variable which type initialized to Any,
/// then for each specific node expression types are constrained by semantics of the expression, and in some cases
/// type variables are unified (two expressions assigned to one type variable). For example, expression "-a" can be
/// unified with expression "a", so it has one type, and in the same time constrained to type "int" or "float", as they
/// are the only types that can be operands to unary minus. Note that unification and constraint rules are provided
/// by concrete language by means of generalization table and analyzeNode() redefinition.
///
/// Remembers the results of type analysis and if called multiple times, takes into account previous results. Note that
/// types in previous expressions can change based on information from next expressions. For example, after analyzing
/// "a = 1" in some language type for "a" will be inferred as "int", but after "a += 0.5" its type will change to
/// "float" (even for first assignment).
class QRTEXT_EXPORT SemanticAnalyzer
{
public:
	/// Constructor. Takes generalizations table for a language and a list of errors to put semantic errors to. Typical
	/// usage pattern is to subclass SemanticAnalyzer for concrete language and provide generalizations table in
	/// constructor there.
	SemanticAnalyzer(QSharedPointer<GeneralizationsTableInterface> const &generalizationsTable, QList<Error> &errors);

	virtual ~SemanticAnalyzer();

	/// Analyzes given tree and assigns types for each found expression. Then types can be retrieved by calling type().
	/// Returns analyzed tree (as of now, it is unmodified).
	QSharedPointer<ast::Node> analyze(QSharedPointer<ast::Node> const &root);

	/// Returns type for a given expression (if that expression was seen by "analyze" method before, or nullptr).
	QSharedPointer<types::TypeExpression> type(QSharedPointer<ast::Node> const &expression) const;

	/// Returns list of identifier names known to semantic analyzer.
	QStringList identifiers() const;

	/// Returns a mapping of variable identifiers known to semantic analyzer to their types.
	QMap<QString, QSharedPointer<core::types::TypeExpression>> variableTypes() const;

	/// Returns true if \a specific type is a subtype of \a general type, including case when they are equivalent.
	bool isGeneralization(const QSharedPointer<core::types::TypeExpression> &specific
			, const QSharedPointer<core::types::TypeExpression> &general) const;

	/// Clears the state of semantic analyzer, forgetting known identifiers and expression types.
	virtual void clear();

	/// Forgets types of an expression and all subexpressions, used when corresponding text is reparsed. Does not forget
	/// types of identifiers declared there.
	void forget(QSharedPointer<ast::Node> const &root);

protected:
	/// Assigns given type to given expression.
	void assign(QSharedPointer<ast::Node> const &expression, const QSharedPointer<types::TypeExpression> &type);

	/// Constrains given expression (passed as "node" parameter) to a given set of types, on behalf of given operation.
	/// Operation is used for error connection purposes only.
	void constrain(QSharedPointer<ast::Node> const &operation
			, QSharedPointer<ast::Node> const &node, QList<QSharedPointer<types::TypeExpression>> const &types);

	/// Unifies left-hand side expression with right-hand side expression, so the type of left expressions becomes the
	/// same type as right expression. These types change together afterwards.
	void unify(QSharedPointer<ast::Node> const &lhs, QSharedPointer<ast::Node> const &rhs);

	/// Reports given semantic error on a given node.
	void reportError(QSharedPointer<ast::Node> const &node, const QString &errorMessage);

	/// Returns true, if given identifier was declared (or seen before).
	bool hasDeclaration(const QString &identifierName) const;

	/// Returns expression where given identifier was declared or encountered first.
	QSharedPointer<ast::Node> declaration(const QString &identifierName) const;

	/// Adds declaration of a given identifier to identifiers table.
	void addDeclaration(const QString &identifierName, QSharedPointer<ast::Node> const &declaration);

	/// Provides Any type constant to descendants.
	const QSharedPointer<types::TypeExpression> &any();

	/// Provides generalizations table for descendants.
	const GeneralizationsTableInterface &generalizationsTable() const;

	/// Provides acces to type variable for given expression to descendants. Note that type() will return resolved type.
	QSharedPointer<types::TypeVariable> typeVariable(QSharedPointer<ast::Node> const &expression) const;

	/// Mark current inferred types as dirty (for example, when type of a variable was changed by generalization)
	/// and request another pass on AST to recheck type constraints.
	void requestRecheck();

private:
	/// Collects type information on a subtree.
	void collect(QSharedPointer<ast::Node> const &node);

	/// Checks that all nodes in AST have their types.
	void finalizeResolve(QSharedPointer<ast::Node> const &node);

	/// Analyzes given node assuming that all its descendants were analysed and provides type information for it
	/// using methods like constrain() and unify(). Shall be defined in concrete analyzers.
	virtual void analyzeNode(QSharedPointer<ast::Node> const &node) = 0;

	/// Called for entire tree to be analyzed, shall be redefined in concrete analyzers if they wish
	/// to do some context-sensitive checks before main "analyzeNode" pass. Default implementation
	/// does nothing.
	virtual void precheck(QSharedPointer<ast::Node> const &node);

	/// Contains mapping from expression to its type. Type is always stored as type variable to make further analysis
	/// more convenient.
	QHash<QSharedPointer<ast::Expression>, QSharedPointer<types::TypeVariable>> mTypes;

	/// Contains mapping from identifier names to their declarations (or first appearance of an identifier).
	QHash<QString, QSharedPointer<ast::Node>> mIdentifierDeclarations;

	/// Special type denoting any type.
	QSharedPointer<types::TypeExpression> mAny;

	/// A list to report errors to.
	QList<Error> &mErrors;

	/// Table with information about relations of types.
	QSharedPointer<GeneralizationsTableInterface> mGeneralizationsTable;

	/// True when we need to traverse AST and check type constraints again.
	bool mRecheckNeeded = true;
};

}
}
