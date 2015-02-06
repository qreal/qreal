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

int PrecedenceConverter::precedence(qrtext::core::ast::Node const &node)
{
	mLastRequestPresedence = otherNode;
	node.accept(*this);
	return mLastRequestPresedence;
}

qrtext::core::Associativity PrecedenceConverter::associativity(qrtext::core::ast::Node const &node)
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

void PrecedenceConverter::visit(qrtext::lua::ast::Not const &)
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

void PrecedenceConverter::visit(qrtext::lua::ast::Subtraction const &)
{
	returnPrecedence(subtraction);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::Multiplication const &)
{
	returnPrecedence(multiplication);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::Division const &)
{
	returnPrecedence(division);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::IntegerDivision const &)
{
	returnPrecedence(division);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::Modulo const &)
{
	returnPrecedence(modulo);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::Exponentiation const &)
{
	returnPrecedence(functionCall);
	returnAssociativity(qrtext::core::Associativity::right);
}

void PrecedenceConverter::visit(qrtext::lua::ast::BitwiseAnd const &)
{
	returnPrecedence(bitwiseAnd);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::BitwiseOr const &)
{
	returnPrecedence(bitwiseOr);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::BitwiseXor const &)
{
	returnPrecedence(bitwiseXor);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::BitwiseLeftShift const &)
{
	returnPrecedence(bitwiseLeftShift);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::BitwiseRightShift const &)
{
	returnPrecedence(bitwiseRightShift);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::Concatenation const &)
{
	returnPrecedence(addition);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::Equality const &)
{
	returnPrecedence(equality);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::LessThan const &)
{
	returnPrecedence(lessThan);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::LessOrEqual const &)
{
	returnPrecedence(lessOrEqual);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::Inequality const &)
{
	returnPrecedence(inequality);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::GreaterThan const &)
{
	returnPrecedence(greaterThan);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::GreaterOrEqual const &)
{
	returnPrecedence(greaterOrEqual);
	returnAssociativity(qrtext::core::Associativity::left);
}
