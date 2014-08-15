#include "textLanguageParser/semanticAnalyzer.h"

#include "textLanguageParser/ast/nodes/assignment.h"
#include "textLanguageParser/ast/nodes/identifier.h"
#include "textLanguageParser/ast/nodes/indexingExpression.h"

#include "textLanguageParser/ast/nodes/unaryMinus.h"

#include "textLanguageParser/ast/nodes/integerNumber.h"
#include "textLanguageParser/ast/nodes/floatNumber.h"
#include "textLanguageParser/ast/nodes/true.h"
#include "textLanguageParser/ast/nodes/false.h"
#include "textLanguageParser/ast/nodes/nil.h"
#include "textLanguageParser/ast/nodes/string.h"

#include "textLanguageParser/types/typeVariable.h"

using namespace textLanguageParser;

SemanticAnalyzer::SemanticAnalyzer(QList<ParserError> &errors)
	: mErrors(errors)
{
	mBoolean = wrap(new types::Boolean());
	mFloat = wrap(new types::Float());
	mInteger = wrap(new types::Integer());
	mNil = wrap(new types::Nil());
	mString = wrap(new types::String());
	mAny = wrap(new types::Any());
}

void SemanticAnalyzer::analyze(QSharedPointer<ast::Node> const &root)
{
	collect(root);
	finalizeResolve(root);
}

void SemanticAnalyzer::collect(QSharedPointer<ast::Node> const &node)
{
	for (auto child : node->children()) {
		if (!child.isNull()) {
			collect(child);
		}
	}

	if (node->is<ast::Assignment>()) {
		auto assignment = as<ast::Assignment>(node);
		constrainAssignment(assignment, assignment->variable(), assignment->value());
	} else if (node->is<ast::Identifier>()) {
		auto identifier = as<ast::Identifier>(node);
		if (mIdentifierDeclarations.contains(identifier->name())) {
			unify(identifier, mIdentifierDeclarations.value(identifier->name()));
		} else {
			assign(identifier, mAny);
			mIdentifierDeclarations.insert(identifier->name(), identifier);
		}
	} else if (node->is<ast::IntegerNumber>()) {
		assign(node, mInteger);
	} else if (node->is<ast::FloatNumber>()) {
		assign(node, mFloat);
	} else if (node->is<ast::True>() || node->is<ast::False>()) {
		assign(node, mBoolean);
	} else if (node->is<ast::String>()) {
		assign(node, mString);
	} else if (node->is<ast::Nil>()) {
		assign(node, mNil);
	} else if (node->is<ast::UnaryMinus>()) {
		auto operand = as<ast::UnaryOperator>(node)->operand();
		constrain(node, operand, {mInteger, mFloat});
		unify(node, operand);
	}
}

void SemanticAnalyzer::finalizeResolve(QSharedPointer<ast::Node> const &node)
{
	if (node->is<ast::Expression>()) {
		auto expression = as<ast::Expression>(node);
		if (mTypes.contains(expression)) {
			auto typeVariable = as<types::TypeVariable>(type(expression));
			if (typeVariable->isResolved()) {
				auto expressionType = typeVariable->finalType();
				mTypes.insert(expression, expressionType);
			} else if (typeVariable->isEmpty()) {
				reportError(expression, QObject::tr("Type mismatch"));
			} else {
				reportError(expression, QObject::tr("Can not deduce type"));
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
		return mTypes.value(castedExpression);
	} else {
		return mAny;
	}
}

void SemanticAnalyzer::assign(QSharedPointer<ast::Node> const &expression
		, QSharedPointer<types::TypeExpression> const &type)
{
	mTypes.insert(as<ast::Expression>(expression), wrap(new types::TypeVariable(type)));
}

void SemanticAnalyzer::unify(QSharedPointer<ast::Node> const &lhs, QSharedPointer<ast::Node> const &rhs)
{
	mTypes.insert(as<ast::Expression>(lhs), type(rhs));
}

void SemanticAnalyzer::constrainAssignment(QSharedPointer<ast::Node> const &operation
		, QSharedPointer<ast::Node> const &lhs, QSharedPointer<ast::Node> const &rhs)
{
	if (!lhs->is<ast::Identifier>() && !lhs->is<ast::IndexingExpression>()) {
		reportError(operation, QObject::tr("Incorrect assignment, only variables and tables can be assigned to."));
		return;
	}

	auto lhsType = as<types::TypeVariable>(type(lhs));
	auto rhsType = as<types::TypeVariable>(type(rhs));
	lhsType->constrain(rhsType, mGeneralizationsTable);
	if (lhsType->isEmpty()) {
		reportError(operation, QObject::tr("Left and right operand have mismatched types."));
	}
}

void SemanticAnalyzer::constrain(QSharedPointer<ast::Node> const &operation
		, QSharedPointer<ast::Node> const &node, QList<QSharedPointer<types::TypeExpression>> const &types)
{
	auto nodeType = as<types::TypeVariable>(type(node));
	nodeType->constrain(types, mGeneralizationsTable);
	if (nodeType->isEmpty()) {
		reportError(operation, QObject::tr("Type mismatch."));
	}
}

void SemanticAnalyzer::reportError(QSharedPointer<ast::Node> const &node, QString const &errorMessage)
{
	mErrors << ParserError(node->start(), errorMessage, ErrorType::semanticError, Severity::error);
}
