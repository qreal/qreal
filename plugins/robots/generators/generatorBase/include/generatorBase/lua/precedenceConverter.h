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

#include "precedenceConverterInterface.h"

#include <qrtext/lua/luaAstVisitorInterface.h>

namespace generatorBase {
namespace lua {

/// Provides converting precedence table base implementation with typical operators precedence.
class ROBOTS_GENERATOR_EXPORT PrecedenceConverter : public PrecedenceConverterInterface
		, public qrtext::lua::LuaAstVisitorInterface
{
public:
	int precedence(const qrtext::core::ast::Node &node) override;
	qrtext::core::Associativity associativity(const qrtext::core::ast::Node &node) override;

private:
	void returnPrecedence(int precedence);
	void returnAssociativity(qrtext::core::Associativity associativity);

	void visit(const qrtext::lua::ast::UnaryMinus &node) override;
	void visit(const qrtext::lua::ast::Not &node) override;
	void visit(const qrtext::lua::ast::BitwiseNegation &node) override;
	void visit(const qrtext::lua::ast::LogicalAnd &node) override;
	void visit(const qrtext::lua::ast::LogicalOr &node) override;
	void visit(const qrtext::lua::ast::Addition &node) override;
	void visit(const qrtext::lua::ast::Subtraction &node) override;
	void visit(const qrtext::lua::ast::Multiplication &node) override;
	void visit(const qrtext::lua::ast::Division &node) override;
	void visit(const qrtext::lua::ast::IntegerDivision &node) override;
	void visit(const qrtext::lua::ast::Modulo &node) override;
	void visit(const qrtext::lua::ast::Exponentiation &node) override;
	void visit(const qrtext::lua::ast::BitwiseAnd &node) override;
	void visit(const qrtext::lua::ast::BitwiseOr &node) override;
	void visit(const qrtext::lua::ast::BitwiseXor &node) override;
	void visit(const qrtext::lua::ast::BitwiseLeftShift &node) override;
	void visit(const qrtext::lua::ast::BitwiseRightShift &node) override;
	void visit(const qrtext::lua::ast::Concatenation &node) override;
	void visit(const qrtext::lua::ast::Equality &node) override;
	void visit(const qrtext::lua::ast::LessThan &node) override;
	void visit(const qrtext::lua::ast::LessOrEqual &node) override;
	void visit(const qrtext::lua::ast::Inequality &node) override;
	void visit(const qrtext::lua::ast::GreaterThan &node) override;
	void visit(const qrtext::lua::ast::GreaterOrEqual &node) override;

	int mLastRequestPresedence;
	qrtext::core::Associativity mLastRequestAssociativity;
};

}
}
