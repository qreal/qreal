#include "precedenceConverter.h"

using namespace generatorBase::lua;

int const otherNode = 100;
int const functionCall = 99;

int const logicalNot = 10;
int const bitwiseNot = 10;
int const unaryPlus = 10;
int const unaryNegation = 10;

int const multiplication = 9;
int const division = 9;
int const modulo = 9;

int const addition = 8;
int const subtraction = 8;

int const bitwiseLeftShift = 7;
int const bitwiseRightShift = 7;

int const lessThan = 6;
int const lessOrEqual = 6;
int const greaterThan = 6;
int const greaterOrEqual = 6;

int const equality = 5;
int const inequality = 5;

int const bitwiseAnd = 4;
int const bitwiseXor = 3;
int const bitwiseOr = 2;
int const logicalAnd = 1;
int const logicalOr = 0;

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

void PrecedenceConverter::visit(qrtext::lua::ast::UnaryMinus const &)
{
	returnPrecedence(unaryNegation);
}

void PrecedenceConverter::visit(qrtext::lua::ast::Not const &)
{
	returnPrecedence(logicalNot);
}

void PrecedenceConverter::visit(qrtext::lua::ast::BitwiseNegation const &)
{
	returnPrecedence(bitwiseNot);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::LogicalAnd const &)
{
	returnPrecedence(logicalAnd);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::LogicalOr const &)
{
	returnPrecedence(logicalOr);
	returnAssociativity(qrtext::core::Associativity::left);
}

void PrecedenceConverter::visit(qrtext::lua::ast::Addition const &)
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
