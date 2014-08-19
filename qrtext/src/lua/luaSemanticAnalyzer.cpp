#include "qrtext/src/lua/luaSemanticAnalyzer.h"

#include "qrtext/src/lua/luaGeneralizationsTable.h"

#include "qrtext/core/types/typeVariable.h"

#include "qrtext/lua/types/boolean.h"
#include "qrtext/lua/types/float.h"
#include "qrtext/lua/types/function.h"
#include "qrtext/lua/types/integer.h"
#include "qrtext/lua/types/nil.h"
#include "qrtext/lua/types/number.h"
#include "qrtext/lua/types/string.h"
#include "qrtext/lua/types/table.h"

#include "qrtext/lua/ast/assignment.h"
#include "qrtext/lua/ast/identifier.h"
#include "qrtext/lua/ast/indexingExpression.h"

#include "qrtext/lua/ast/unaryMinus.h"

#include "qrtext/lua/ast/integerNumber.h"
#include "qrtext/lua/ast/floatNumber.h"
#include "qrtext/lua/ast/true.h"
#include "qrtext/lua/ast/false.h"
#include "qrtext/lua/ast/nil.h"
#include "qrtext/lua/ast/string.h"

using namespace qrtext::lua::details;
using namespace qrtext::core;

LuaSemanticAnalyzer::LuaSemanticAnalyzer(QList<Error> &errors)
	: SemanticAnalyzer(QSharedPointer<GeneralizationsTableInterface>(new LuaGeneralizationsTable()), errors)
{
	mBoolean = core::wrap(new types::Boolean());
	mFloat = core::wrap(new types::Float());
	mInteger = core::wrap(new types::Integer());
	mNil = core::wrap(new types::Nil());
	mString = core::wrap(new types::String());
}

void LuaSemanticAnalyzer::analyzeNode(QSharedPointer<core::ast::Node> const &node)
{
	if (node->is<ast::Assignment>()) {
		auto assignment = as<ast::Assignment>(node);
		constrainAssignment(assignment, assignment->variable(), assignment->value());
	} else if (node->is<ast::Identifier>()) {
		auto identifier = as<ast::Identifier>(node);
		if (hasDeclaration(identifier->name())) {
			unify(identifier, declaration(identifier->name()));
		} else {
			assign(identifier, any());
			addDeclaration(identifier->name(), identifier);
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
		auto operand = as<core::ast::UnaryOperator>(node)->operand();
		constrain(node, operand, {mInteger, mFloat});
		unify(node, operand);
	}
}

void LuaSemanticAnalyzer::constrainAssignment(QSharedPointer<core::ast::Node> const &operation
		, QSharedPointer<core::ast::Node> const &lhs, QSharedPointer<core::ast::Node> const &rhs)
{
	if (!lhs->is<ast::Identifier>() && !lhs->is<ast::IndexingExpression>()) {
		reportError(operation, QObject::tr("Incorrect assignment, only variables and tables can be assigned to."));
		return;
	}

	auto lhsType = as<core::types::TypeVariable>(type(lhs));
	auto rhsType = as<core::types::TypeVariable>(type(rhs));
	lhsType->constrain(rhsType, generalizationsTable());
	if (lhsType->isEmpty()) {
		reportError(operation, QObject::tr("Left and right operand have mismatched types."));
	}
}
