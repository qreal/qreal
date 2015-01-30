#pragma once

#include "qrtext/core/astVisitorInterface.h"

namespace qrtext {
namespace lua {

namespace ast {
class Number;
class UnaryMinus;
class Not;
class BitwiseNegation;
class Length;
class LogicalAnd;
class LogicalOr;
class Addition;
class Subtraction;
class Multiplication;
class Division;
class IntegerDivision;
class Modulo;
class Exponentiation;
class BitwiseAnd;
class BitwiseOr;
class BitwiseXor;
class BitwiseLeftShift;
class BitwiseRightShift;
class Concatenation;
class Equality;
class LessThan;
class LessOrEqual;
class Inequality;
class GreaterThan;
class GreaterOrEqual;
class IntegerNumber;
class FloatNumber;
class FieldInitialization;
class TableConstructor;
class IntegerNumber;
class FloatNumber;
class String;
class True;
class False;
class Nil;
class Identifier;
class FunctionCall;
class MethodCall;
class Assignment;
class Block;
class IndexingExpression;
}

/// An interface for every entity that wants to operate with lua language AST.
/// See 'visitor' design pattern (http://www.oodesign.com/visitor-pattern.html).
class LuaAstVisitorInterface : public core::AstVisitorInterface
{
public:
	~LuaAstVisitorInterface() override {}

	virtual void visit(ast::Number const &node)              { Q_UNUSED(node); }
	virtual void visit(ast::UnaryMinus const &node)          { Q_UNUSED(node); }
	virtual void visit(ast::Not const &node)                 { Q_UNUSED(node); }
	virtual void visit(ast::BitwiseNegation const &node)     { Q_UNUSED(node); }
	virtual void visit(ast::Length const &node)              { Q_UNUSED(node); }
	virtual void visit(ast::LogicalAnd const &node)          { Q_UNUSED(node); }
	virtual void visit(ast::LogicalOr const &node)           { Q_UNUSED(node); }
	virtual void visit(ast::Addition const &node)            { Q_UNUSED(node); }
	virtual void visit(ast::Subtraction const &node)         { Q_UNUSED(node); }
	virtual void visit(ast::Multiplication const &node)      { Q_UNUSED(node); }
	virtual void visit(ast::Division const &node)            { Q_UNUSED(node); }
	virtual void visit(ast::IntegerDivision const &node)     { Q_UNUSED(node); }
	virtual void visit(ast::Modulo const &node)              { Q_UNUSED(node); }
	virtual void visit(ast::Exponentiation const &node)      { Q_UNUSED(node); }
	virtual void visit(ast::BitwiseAnd const &node)          { Q_UNUSED(node); }
	virtual void visit(ast::BitwiseOr const &node)           { Q_UNUSED(node); }
	virtual void visit(ast::BitwiseXor const &node)          { Q_UNUSED(node); }
	virtual void visit(ast::BitwiseLeftShift const &node)    { Q_UNUSED(node); }
	virtual void visit(ast::BitwiseRightShift const &node)   { Q_UNUSED(node); }
	virtual void visit(ast::Concatenation const &node)       { Q_UNUSED(node); }
	virtual void visit(ast::Equality const &node)            { Q_UNUSED(node); }
	virtual void visit(ast::LessThan const &node)            { Q_UNUSED(node); }
	virtual void visit(ast::LessOrEqual const &node)         { Q_UNUSED(node); }
	virtual void visit(ast::Inequality const &node)          { Q_UNUSED(node); }
	virtual void visit(ast::GreaterThan const &node)         { Q_UNUSED(node); }
	virtual void visit(ast::GreaterOrEqual const &node)      { Q_UNUSED(node); }
	virtual void visit(ast::IntegerNumber const &node)       { Q_UNUSED(node); }
	virtual void visit(ast::FloatNumber const &node)         { Q_UNUSED(node); }
	virtual void visit(ast::FieldInitialization const &node) { Q_UNUSED(node); }
	virtual void visit(ast::TableConstructor const &node)    { Q_UNUSED(node); }
	virtual void visit(ast::String const &node)              { Q_UNUSED(node); }
	virtual void visit(ast::True const &node)                { Q_UNUSED(node); }
	virtual void visit(ast::False const &node)               { Q_UNUSED(node); }
	virtual void visit(ast::Nil const &node)                 { Q_UNUSED(node); }
	virtual void visit(ast::Identifier const &node)          { Q_UNUSED(node); }
	virtual void visit(ast::FunctionCall const &node)        { Q_UNUSED(node); }
	virtual void visit(ast::MethodCall const &node)          { Q_UNUSED(node); }
	virtual void visit(ast::Assignment const &node)          { Q_UNUSED(node); }
	virtual void visit(ast::Block const &node)               { Q_UNUSED(node); }
	virtual void visit(ast::IndexingExpression const &node)  { Q_UNUSED(node); }

	void visit(core::ast::Node const &node) override           { Q_UNUSED(node); }
	void visit(core::ast::Expression const &node) override     { Q_UNUSED(node); }
	void visit(core::ast::BinaryOperator const &node) override { Q_UNUSED(node); }
	void visit(core::ast::UnaryOperator const &node) override  { Q_UNUSED(node); }

};

}
}
