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

#include "qrtext/core/semantics/semanticAnalyzer.h"

#include "qrtext/core/types/any.h"

using namespace qrtext::core;

SemanticAnalyzer::SemanticAnalyzer(QSharedPointer<GeneralizationsTableInterface> const &generalizationsTable
	, QList<Error> &errors)
	: mAny(new types::Any()), mErrors(errors), mGeneralizationsTable(generalizationsTable)
{
}

SemanticAnalyzer::~SemanticAnalyzer()
{
}

QSharedPointer<ast::Node> SemanticAnalyzer::analyze(QSharedPointer<ast::Node> const &root)
{
	if (!root) {
		return root;
	}

	precheck(root);

	mRecheckNeeded = true;

	while (mRecheckNeeded) {
		mRecheckNeeded = false;
		collect(root);
	}

	finalizeResolve(root);
	return root;
}

void SemanticAnalyzer::collect(QSharedPointer<ast::Node> const &node)
{
	for (const auto &child : node->children()) {
		if (!child.isNull()) {
			collect(child);
		}
	}

	analyzeNode(node);
}

void SemanticAnalyzer::finalizeResolve(const QSharedPointer<ast::Node> &node)
{
	if (node->is<ast::Expression>()) {
		const auto expression = as<ast::Expression>(node);
		if (mTypes.contains(expression)) {
			const QSharedPointer<types::TypeVariable> &typeVariable = mTypes.value(expression);
			if (typeVariable->isEmpty()) {
				reportError(expression, QObject::tr("Type mismatch"));
			} else if (!typeVariable->isResolved()) {
				if (typeVariable->finalType() == mAny) {
					// Do nothing, error shall already be reported as unknown identifier.
				} else {
					const QString error = QObject::tr("Can not deduce type, expression can be of following types: %1")
							.arg(typeVariable->toString());
					reportError(expression, error);
				}
			}
		}
	}

	for (auto child : node->children()) {
		if (!child.isNull()) {
			finalizeResolve(child);
		}
	}
}

QSharedPointer<types::TypeExpression> SemanticAnalyzer::type(QSharedPointer<ast::Node> const &expression) const
{
	auto castedExpression = as<ast::Expression>(expression);
	if (mTypes.contains(castedExpression)) {
		return mTypes.value(castedExpression)->finalType();
	} else {
		return mAny;
	}
}

QStringList SemanticAnalyzer::identifiers() const
{
	return mIdentifierDeclarations.keys();
}

QMap<QString, QSharedPointer<types::TypeExpression> > SemanticAnalyzer::variableTypes() const
{
	QMap<QString, QSharedPointer<qrtext::core::types::TypeExpression>> result;
	for (const QString &identifier : mIdentifierDeclarations.keys()) {
		result[identifier] = type(mIdentifierDeclarations[identifier]);
	}

	return result;
}

void SemanticAnalyzer::clear()
{
	mTypes.clear();
	mIdentifierDeclarations.clear();
}

void SemanticAnalyzer::forget(const QSharedPointer<ast::Node> &root)
{
	if (!root) {
		return;
	}

	if (!mIdentifierDeclarations.values().contains(root)) {
		const auto expression = root.dynamicCast<ast::Expression>();
		if (expression) {
			mTypes.remove(expression);
		}
	}

	for (const auto &child : root->children()) {
		if (!child.isNull()) {
			forget(child);
		}
	}
}

void SemanticAnalyzer::assign(const QSharedPointer<ast::Node> &expression
		, const QSharedPointer<types::TypeExpression> &type)
{
	const auto castExpression = as<ast::Expression>(expression);

	if (!type->is<types::TypeVariable>()) {
		if (mTypes.contains(castExpression)) {
			// If type variable for that expression already exists, we must constrain it rather than create
			// new variable. Else it doesn't play well with coercion --- variable gets created, then coerced,
			// then program is rechecked to verify coercion results, new variable is created during recheck,
			// gets coerced and so on, infinitely.
			mTypes[castExpression]->constrain(QList<QSharedPointer<types::TypeExpression>>{type}
					, *mGeneralizationsTable);
		} else {
			mTypes.insert(castExpression, QSharedPointer<types::TypeVariable>(new types::TypeVariable(type)));
		}
	} else {
		mTypes.insert(castExpression, type.dynamicCast<types::TypeVariable>());
	}
}

void SemanticAnalyzer::unify(QSharedPointer<ast::Node> const &lhs, QSharedPointer<ast::Node> const &rhs)
{
	mTypes.insert(as<ast::Expression>(lhs), mTypes.value(as<ast::Expression>(rhs)));
}

void SemanticAnalyzer::constrain(QSharedPointer<ast::Node> const &operation
		, QSharedPointer<ast::Node> const &node, QList<QSharedPointer<types::TypeExpression>> const &types)
{
	auto nodeType = mTypes.value(as<ast::Expression>(node));
	if (!nodeType) {
		reportError(node, QObject::tr("This construction is not supported by semantic analysis"));
		return;
	}

	nodeType->constrain(types, generalizationsTable());
	if (nodeType->isEmpty()) {
		reportError(operation, QObject::tr("Type mismatch."));
	}
}

void SemanticAnalyzer::reportError(QSharedPointer<ast::Node> const &node, const QString &errorMessage)
{
	mErrors << Error(node->start(), errorMessage, ErrorType::semanticError, Severity::error);
}

bool SemanticAnalyzer::hasDeclaration(const QString &identifierName) const
{
	return mIdentifierDeclarations.contains(identifierName);
}

QSharedPointer<ast::Node> SemanticAnalyzer::declaration(const QString &identifierName) const
{
	return mIdentifierDeclarations.value(identifierName);
}

void SemanticAnalyzer::addDeclaration(const QString &identifierName, QSharedPointer<ast::Node> const &declaration)
{
	mIdentifierDeclarations.insert(identifierName, declaration);
}

const QSharedPointer<types::TypeExpression> &SemanticAnalyzer::any()
{
	return mAny;
}

const GeneralizationsTableInterface &SemanticAnalyzer::generalizationsTable() const
{
	return *mGeneralizationsTable;
}

QSharedPointer<types::TypeVariable> SemanticAnalyzer::typeVariable(QSharedPointer<ast::Node> const &expression) const
{
	return mTypes.value(as<ast::Expression>(expression));
}

void SemanticAnalyzer::requestRecheck()
{
	mRecheckNeeded = true;
}

bool SemanticAnalyzer::isGeneralization(const QSharedPointer<types::TypeExpression> &specific
		, const QSharedPointer<types::TypeExpression> &general) const
{
	return generalizationsTable().isGeneralization(specific, general);
}

void SemanticAnalyzer::precheck(QSharedPointer<ast::Node> const &node)
{
	Q_UNUSED(node)
}
