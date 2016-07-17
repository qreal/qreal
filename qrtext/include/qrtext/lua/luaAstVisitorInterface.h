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

	// Second argument is a parent everywhere.

	virtual void visit(const QSharedPointer<ast::Number> &             , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::UnaryMinus> &         , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::Not> &                , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::BitwiseNegation> &    , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::Length> &             , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::LogicalAnd> &         , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::LogicalOr> &          , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::Addition> &           , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::Subtraction> &        , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::Multiplication> &     , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::Division> &           , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::IntegerDivision> &    , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::Modulo> &             , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::Exponentiation> &     , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::BitwiseAnd> &         , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::BitwiseOr> &          , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::BitwiseXor> &         , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::BitwiseLeftShift> &   , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::BitwiseRightShift> &  , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::Concatenation> &      , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::Equality> &           , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::LessThan> &           , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::LessOrEqual> &        , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::Inequality> &         , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::GreaterThan> &        , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::GreaterOrEqual> &     , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::IntegerNumber> &      , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::FloatNumber> &        , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::FieldInitialization> &, const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::TableConstructor> &   , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::String> &             , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::True> &               , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::False> &              , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::Nil> &                , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::Identifier> &         , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::FunctionCall> &       , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::MethodCall> &         , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::Assignment> &         , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::Block> &              , const QSharedPointer<core::ast::Node> &) {}
	virtual void visit(const QSharedPointer<ast::IndexingExpression> & , const QSharedPointer<core::ast::Node> &) {}

	void visit(const QSharedPointer<core::ast::Node> &          , const QSharedPointer<core::ast::Node> &) override {}
	void visit(const QSharedPointer<core::ast::Expression> &    , const QSharedPointer<core::ast::Node> &) override {}
	void visit(const QSharedPointer<core::ast::BinaryOperator> &, const QSharedPointer<core::ast::Node> &) override {}
	void visit(const QSharedPointer<core::ast::UnaryOperator> & , const QSharedPointer<core::ast::Node> &) override {}

};

}
}
