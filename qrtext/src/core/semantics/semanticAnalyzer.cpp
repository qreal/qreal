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
			QSharedPointer<types::TypeVariable> const &typeVariable = mTypes.value(expression);
			if (typeVariable->isEmpty()) {
				reportError(expression, QObject::tr("Type mismatch"));
			} else if (!typeVariable->isResolved()) {
				if (typeVariable->finalType() == mAny) {
					reportError(expression, QObject::tr("Can not deduce type, this expression can be of any type"));
				} else {
					QString const error = QObject::tr("Can not deduce type, expression can be of following types: %1")
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
	for (QString const &identifier : mIdentifierDeclarations.keys()) {
		result[identifier] = type(mIdentifierDeclarations[identifier]);
	}

	return result;
}

void SemanticAnalyzer::clear()
{
	mTypes.clear();
	mIdentifierDeclarations.clear();
}

void SemanticAnalyzer::forget(QSharedPointer<ast::Node> const &root)
{
	if (!root) {
		return;
	}

	if (!mIdentifierDeclarations.values().contains(root)) {
		auto const expression = root.dynamicCast<ast::Expression>();
		if (expression) {
			mTypes.remove(expression);
		}
	}

	for (auto const &child : root->children()) {
		if (!child.isNull()) {
			forget(child);
		}
	}
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
	if (!nodeType) {
		reportError(node, QObject::tr("This construction is not supported by semantic analysis"));
		return;
	}

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
