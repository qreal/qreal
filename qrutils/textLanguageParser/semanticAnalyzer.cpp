#include "textLanguageParser/semanticAnalyzer.h"

#include "textLanguageParser/ast/nodes/assignment.h"
#include "textLanguageParser/ast/nodes/identifier.h"

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
	resolveEquations();
	finalizeResolve(root);
}

void SemanticAnalyzer::collect(QSharedPointer<ast::Node> const &node)
{
	if (node->is<ast::Assignment>()) {
		auto assignment = as<ast::Assignment>(node);
		collect(assignment->variable());
		collect(assignment->value());
		mEquations << types::TypeEquation(assignment->variable(), assignment->value(), types::TypeRelation::assignable);
	} else if (node->is<ast::Identifier>()) {
		assign(node, mAny);
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
	} else if (!node->is<ast::Expression>()) {
		for (auto child : node->children()) {
			collect(child);
		}
	}
}

void SemanticAnalyzer::resolveEquations()
{
	for (types::TypeEquation const &equation : mEquations) {
		auto left = as<types::TypeVariable>(type(equation.left()));
		auto right = as<types::TypeVariable>(type(equation.left()));
		switch (equation.relation()) {
		case types::TypeRelation::assignable:
			constrainAssignment(equation.left(), equation.right());
			break;
		case types::TypeRelation::convertable:
			constrainCast(equation.left(), equation.right());
			break;
		case types::TypeRelation::equal:
			constrainStructuralEquality(left, right);
			break;
		}
	}
}

void SemanticAnalyzer::finalizeResolve(QSharedPointer<ast::Node> const &node)
{
	if (node->is<ast::Expression>()) {
		auto expression = as<ast::Expression>(node);
		if (mTypes.contains(expression)) {
			if (as<types::TypeVariable>(type(expression))->isResolved()) {
				auto expressionType = as<types::TypeVariable>(type(expression))->finalType();
				mTypes.insert(expression, expressionType);
			}
		}
	}

	for (auto child : node->children()) {
		finalizeResolve(child);
	}
}

const QSharedPointer<types::TypeExpression> &SemanticAnalyzer::type(QSharedPointer<ast::Node> const &expression) const
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

void SemanticAnalyzer::constrainAssignment(QSharedPointer<ast::Node> const &lhs, QSharedPointer<ast::Node> const &rhs)
{
}

void SemanticAnalyzer::constrainCast(QSharedPointer<ast::Node> const &left, QSharedPointer<ast::Node> const &right)
{
}

void SemanticAnalyzer::constrainStructuralEquality(QSharedPointer<types::TypeExpression> const &left
		, QSharedPointer<types::TypeExpression> const &right)
{
}
