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

#include "generatorBase/lua/precedenceConverter.h"

using namespace generatorBase::lua;

const int otherNode = 100;
const int functionCall = 99;

const int logicalNot = 10;
const int bitwiseNot = 10;
const int unaryPlus = 10;
const int unaryNegation = 10;

const int multiplication = 9;
const int division = 9;
const int modulo = 9;

const int addition = 8;
const int subtraction = 8;

const int bitwiseLeftShift = 7;
const int bitwiseRightShift = 7;

const int lessThan = 6;
const int lessOrEqual = 6;
const int greaterThan = 6;
const int greaterOrEqual = 6;

const int equality = 5;
const int inequality = 5;

const int bitwiseAnd = 4;
const int bitwiseXor = 3;
const int bitwiseOr = 2;
const int logicalAnd = 1;
const int logicalOr = 0;

int PrecedenceConverter::precedence(const QSharedPointer<qrtext::core::ast::Node> &node)
{
	mLastRequestPresedence = otherNode;
	node->accept(*this, node, qrtext::wrap(nullptr));
	return mLastRequestPresedence;
}

qrtext::core::Associativity PrecedenceConverter::associativity(const QSharedPointer<qrtext::core::ast::Node> &node)
{
	mLastRequestAssociativity = qrtext::core::Associativity::right;
	node->accept(*this, node, qrtext::wrap(nullptr));
	return mLastRequestAssociativity;
}

void PrecedenceConverter::returnPrecedence(int precedence)
{
	mLastRequestPresedence = precedence;
}

void PrecedenceConverter::returnAssociativity(qrtext::core::Associativity associativity)
{
	mLastRequestAssociativity = associativity;
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::UnaryMinus> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(unaryNegation);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::Not> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(logicalNot);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseNegation> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(bitwiseNot);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::LogicalAnd> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(logicalAnd);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::LogicalOr> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(logicalOr);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::Addition> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(addition);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::Subtraction> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(subtraction);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::Multiplication> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(multiplication);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::Division> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(division);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::IntegerDivision> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(division);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::Modulo> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(modulo);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::Exponentiation> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(functionCall);
	returnAssociativity(qrtext::core::Associativity::right);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseAnd> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(bitwiseAnd);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseOr> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(bitwiseOr);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseXor> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(bitwiseXor);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseLeftShift> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(bitwiseLeftShift);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseRightShift> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(bitwiseRightShift);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::Concatenation> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(addition);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::Equality> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(equality);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::LessThan> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(lessThan);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::LessOrEqual> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(lessOrEqual);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::Inequality> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(inequality);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::GreaterThan> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(greaterThan);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const QSharedPointer<qrtext::lua::ast::GreaterOrEqual> &
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	returnPrecedence(greaterOrEqual);
	returnAssociativity(qrtext::core::Associativity::left);
}
