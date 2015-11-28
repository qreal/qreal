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

	virtual void visit(const QSharedPointer<ast::Number> &)              {}
	virtual void visit(const QSharedPointer<ast::UnaryMinus> &)          {}
	virtual void visit(const QSharedPointer<ast::Not> &)                 {}
	virtual void visit(const QSharedPointer<ast::BitwiseNegation> &)     {}
	virtual void visit(const QSharedPointer<ast::Length> &)              {}
	virtual void visit(const QSharedPointer<ast::LogicalAnd> &)          {}
	virtual void visit(const QSharedPointer<ast::LogicalOr> &)           {}
	virtual void visit(const QSharedPointer<ast::Addition> &)            {}
	virtual void visit(const QSharedPointer<ast::Subtraction> &)         {}
	virtual void visit(const QSharedPointer<ast::Multiplication> &)      {}
	virtual void visit(const QSharedPointer<ast::Division> &)            {}
	virtual void visit(const QSharedPointer<ast::IntegerDivision> &)     {}
	virtual void visit(const QSharedPointer<ast::Modulo> &)              {}
	virtual void visit(const QSharedPointer<ast::Exponentiation> &)      {}
	virtual void visit(const QSharedPointer<ast::BitwiseAnd> &)          {}
	virtual void visit(const QSharedPointer<ast::BitwiseOr> &)           {}
	virtual void visit(const QSharedPointer<ast::BitwiseXor> &)          {}
	virtual void visit(const QSharedPointer<ast::BitwiseLeftShift> &)    {}
	virtual void visit(const QSharedPointer<ast::BitwiseRightShift> &)   {}
	virtual void visit(const QSharedPointer<ast::Concatenation> &)       {}
	virtual void visit(const QSharedPointer<ast::Equality> &)            {}
	virtual void visit(const QSharedPointer<ast::LessThan> &)            {}
	virtual void visit(const QSharedPointer<ast::LessOrEqual> &)         {}
	virtual void visit(const QSharedPointer<ast::Inequality> &)          {}
	virtual void visit(const QSharedPointer<ast::GreaterThan> &)         {}
	virtual void visit(const QSharedPointer<ast::GreaterOrEqual> &)      {}
	virtual void visit(const QSharedPointer<ast::IntegerNumber> &)       {}
	virtual void visit(const QSharedPointer<ast::FloatNumber> &)         {}
	virtual void visit(const QSharedPointer<ast::FieldInitialization> &) {}
	virtual void visit(const QSharedPointer<ast::TableConstructor> &)    {}
	virtual void visit(const QSharedPointer<ast::String> &)              {}
	virtual void visit(const QSharedPointer<ast::True> &)                {}
	virtual void visit(const QSharedPointer<ast::False> &)               {}
	virtual void visit(const QSharedPointer<ast::Nil> &)                 {}
	virtual void visit(const QSharedPointer<ast::Identifier> &)          {}
	virtual void visit(const QSharedPointer<ast::FunctionCall> &)        {}
	virtual void visit(const QSharedPointer<ast::MethodCall> &)          {}
	virtual void visit(const QSharedPointer<ast::Assignment> &)          {}
	virtual void visit(const QSharedPointer<ast::Block> &)               {}
	virtual void visit(const QSharedPointer<ast::IndexingExpression> &)  {}

	void visit(const QSharedPointer<core::ast::Node> &) override           {}
	void visit(const QSharedPointer<core::ast::Expression> &) override     {}
	void visit(const QSharedPointer<core::ast::BinaryOperator> &) override {}
	void visit(const QSharedPointer<core::ast::UnaryOperator> &) override  {}

};

}
}
