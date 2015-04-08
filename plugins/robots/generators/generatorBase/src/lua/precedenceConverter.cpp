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

int PrecedenceConverter::precedence(const qrtext::core::ast::Node &node)
{
	mLastRequestPresedence = otherNode;
	node.accept(*this);
	return mLastRequestPresedence;
}

qrtext::core::Associativity PrecedenceConverter::associativity(const qrtext::core::ast::Node &node)
{
	mLastRequestAssociativity = qrtext::core::Associativity::right;
	node.accept(*this);
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

void PrecedenceConverter::visit(const qrtext::lua::ast::UnaryMinus &)
{
	returnPrecedence(unaryNegation);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::Not &)
{
	returnPrecedence(logicalNot);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::BitwiseNegation &)
{
	returnPrecedence(bitwiseNot);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::LogicalAnd &)
{
	returnPrecedence(logicalAnd);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::LogicalOr &)
{
	returnPrecedence(logicalOr);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::Addition &)
{
	returnPrecedence(addition);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::Subtraction &)
{
	returnPrecedence(subtraction);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::Multiplication &)
{
	returnPrecedence(multiplication);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::Division &)
{
	returnPrecedence(division);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::IntegerDivision &)
{
	returnPrecedence(division);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::Modulo &)
{
	returnPrecedence(modulo);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::Exponentiation &)
{
	returnPrecedence(functionCall);
	returnAssociativity(qrtext::core::Associativity::right);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::BitwiseAnd &)
{
	returnPrecedence(bitwiseAnd);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::BitwiseOr &)
{
	returnPrecedence(bitwiseOr);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::BitwiseXor &)
{
	returnPrecedence(bitwiseXor);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::BitwiseLeftShift &)
{
	returnPrecedence(bitwiseLeftShift);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::BitwiseRightShift &)
{
	returnPrecedence(bitwiseRightShift);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::Concatenation &)
{
	returnPrecedence(addition);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::Equality &)
{
	returnPrecedence(equality);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::LessThan &)
{
	returnPrecedence(lessThan);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::LessOrEqual &)
{
	returnPrecedence(lessOrEqual);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::Inequality &)
{
	returnPrecedence(inequality);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::GreaterThan &)
{
	returnPrecedence(greaterThan);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(const qrtext::lua::ast::GreaterOrEqual &)
{
	returnPrecedence(greaterOrEqual);
	returnAssociativity(qrtext::core::Associativity::left);
}
