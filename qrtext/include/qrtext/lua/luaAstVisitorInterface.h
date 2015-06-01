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

	virtual void visit(const ast::Number &node)              { Q_UNUSED(node); }
	virtual void visit(const ast::UnaryMinus &node)          { Q_UNUSED(node); }
	virtual void visit(const ast::Not &node)                 { Q_UNUSED(node); }
	virtual void visit(const ast::BitwiseNegation &node)     { Q_UNUSED(node); }
	virtual void visit(const ast::Length &node)              { Q_UNUSED(node); }
	virtual void visit(const ast::LogicalAnd &node)          { Q_UNUSED(node); }
	virtual void visit(const ast::LogicalOr &node)           { Q_UNUSED(node); }
	virtual void visit(const ast::Addition &node)            { Q_UNUSED(node); }
	virtual void visit(const ast::Subtraction &node)         { Q_UNUSED(node); }
	virtual void visit(const ast::Multiplication &node)      { Q_UNUSED(node); }
	virtual void visit(const ast::Division &node)            { Q_UNUSED(node); }
	virtual void visit(const ast::IntegerDivision &node)     { Q_UNUSED(node); }
	virtual void visit(const ast::Modulo &node)              { Q_UNUSED(node); }
	virtual void visit(const ast::Exponentiation &node)      { Q_UNUSED(node); }
	virtual void visit(const ast::BitwiseAnd &node)          { Q_UNUSED(node); }
	virtual void visit(const ast::BitwiseOr &node)           { Q_UNUSED(node); }
	virtual void visit(const ast::BitwiseXor &node)          { Q_UNUSED(node); }
	virtual void visit(const ast::BitwiseLeftShift &node)    { Q_UNUSED(node); }
	virtual void visit(const ast::BitwiseRightShift &node)   { Q_UNUSED(node); }
	virtual void visit(const ast::Concatenation &node)       { Q_UNUSED(node); }
	virtual void visit(const ast::Equality &node)            { Q_UNUSED(node); }
	virtual void visit(const ast::LessThan &node)            { Q_UNUSED(node); }
	virtual void visit(const ast::LessOrEqual &node)         { Q_UNUSED(node); }
	virtual void visit(const ast::Inequality &node)          { Q_UNUSED(node); }
	virtual void visit(const ast::GreaterThan &node)         { Q_UNUSED(node); }
	virtual void visit(const ast::GreaterOrEqual &node)      { Q_UNUSED(node); }
	virtual void visit(const ast::IntegerNumber &node)       { Q_UNUSED(node); }
	virtual void visit(const ast::FloatNumber &node)         { Q_UNUSED(node); }
	virtual void visit(const ast::FieldInitialization &node) { Q_UNUSED(node); }
	virtual void visit(const ast::TableConstructor &node)    { Q_UNUSED(node); }
	virtual void visit(const ast::String &node)              { Q_UNUSED(node); }
	virtual void visit(const ast::True &node)                { Q_UNUSED(node); }
	virtual void visit(const ast::False &node)               { Q_UNUSED(node); }
	virtual void visit(const ast::Nil &node)                 { Q_UNUSED(node); }
	virtual void visit(const ast::Identifier &node)          { Q_UNUSED(node); }
	virtual void visit(const ast::FunctionCall &node)        { Q_UNUSED(node); }
	virtual void visit(const ast::MethodCall &node)          { Q_UNUSED(node); }
	virtual void visit(const ast::Assignment &node)          { Q_UNUSED(node); }
	virtual void visit(const ast::Block &node)               { Q_UNUSED(node); }
	virtual void visit(const ast::IndexingExpression &node)  { Q_UNUSED(node); }

	void visit(core::ast::Node const &node) override           { Q_UNUSED(node); }
	void visit(core::ast::Expression const &node) override     { Q_UNUSED(node); }
	void visit(core::ast::BinaryOperator const &node) override { Q_UNUSED(node); }
	void visit(core::ast::UnaryOperator const &node) override  { Q_UNUSED(node); }

};

}
}
