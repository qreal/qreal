#include "qrtext/core/semantics/semanticAnalyzer.h"

//#include "textLanguageParser/ast/nodes/assignment.h"
//#include "textLanguageParser/ast/nodes/identifier.h"
//#include "textLanguageParser/ast/nodes/indexingExpression.h"

//#include "textLanguageParser/ast/nodes/unaryMinus.h"

//#include "textLanguageParser/ast/nodes/integerNumber.h"
//#include "textLanguageParser/ast/nodes/floatNumber.h"
//#include "textLanguageParser/ast/nodes/true.h"
//#include "textLanguageParser/ast/nodes/false.h"
//#include "textLanguageParser/ast/nodes/nil.h"
//#include "textLanguageParser/ast/nodes/string.h"

#include "qrtext/core/types/typeVariable.h"

using namespace qrtext::core;

SemanticAnalyzer::SemanticAnalyzer(QSharedPointer<GeneralizationsTableInterface> const &generalizationsTable
	, QList<Error> &errors)
	: mErrors(errors), mGeneralizationsTable(generalizationsTable)
{
//	mBoolean = wrap(new types::Boolean());
//	mFloat = wrap(new types::Float());
//	mInteger = wrap(new types::Integer());
//	mNil = wrap(new types::Nil());
//	mString = wrap(new types::String());
//	mAny = wrap(new types::Any());
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

	analyzeNode(node);
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

void SemanticAnalyzer::constrain(QSharedPointer<ast::Node> const &operation
		, QSharedPointer<ast::Node> const &node, QList<QSharedPointer<types::TypeExpression>> const &types)
{
	auto nodeType = as<types::TypeVariable>(type(node));
//	nodeType->constrain(types, mGeneralizationsTable);
	if (nodeType->isEmpty()) {
		reportError(operation, QObject::tr("Type mismatch."));
	}
}

void SemanticAnalyzer::reportError(QSharedPointer<ast::Node> const &node, QString const &errorMessage)
{
	mErrors << Error(node->start(), errorMessage, ErrorType::semanticError, Severity::error);
}

bool SemanticAnalyzer::hasDeclaration(QString const &identifierName) const
{
	return mIdentifierDeclarations.contains(identifierName);
}

QSharedPointer<ast::Node> const &SemanticAnalyzer::declaration(QString const &identifierName) const
{
	return mIdentifierDeclarations.value(identifierName);
}

void SemanticAnalyzer::addDeclaration(QString const &identifierName, QSharedPointer<ast::Node> const &declaration)
{
	mIdentifierDeclarations.insert(identifierName, declaration);
}

QSharedPointer<types::TypeExpression> const &SemanticAnalyzer::any()
{
	return mAny;
}

GeneralizationsTableInterface const &SemanticAnalyzer::generalizationsTable() const
{
	return *mGeneralizationsTable;
}
