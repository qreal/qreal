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
			auto typeVariable = mTypes.value(expression);
			if (!typeVariable->isResolved()) {
				reportError(expression, QObject::tr("Can not deduce type"));
			} else if (typeVariable->isEmpty()) {
				reportError(expression, QObject::tr("Type mismatch"));
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

void SemanticAnalyzer::assign(QSharedPointer<ast::Node> const &expression
		, QSharedPointer<types::TypeExpression> const &type)
{
	if (!type->is<types::TypeVariable>()) {
		mTypes.insert(as<ast::Expression>(expression)
				, QSharedPointer<types::TypeVariable>(new types::TypeVariable(type)));
	} else {
		mTypes.insert(as<ast::Expression>(expression), type.dynamicCast<types::TypeVariable>());
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
	nodeType->constrain(types, generalizationsTable());
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

QSharedPointer<ast::Node> SemanticAnalyzer::declaration(QString const &identifierName) const
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

QSharedPointer<types::TypeVariable> SemanticAnalyzer::typeVariable(QSharedPointer<ast::Node> const &expression) const
{
	return mTypes.value(as<ast::Expression>(expression));
}

void SemanticAnalyzer::requestRecheck()
{
	mRecheckNeeded = true;
}
