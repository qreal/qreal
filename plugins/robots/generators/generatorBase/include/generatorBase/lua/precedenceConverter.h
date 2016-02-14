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

#include "precedenceConverterInterface.h"

#include <qrtext/lua/luaAstVisitorInterface.h>

namespace generatorBase {
namespace lua {

/// Provides converting precedence table base implementation with typical operators precedence.
class ROBOTS_GENERATOR_EXPORT PrecedenceConverter : public PrecedenceConverterInterface
		, public qrtext::lua::LuaAstVisitorInterface
{
public:
	int precedence(const QSharedPointer<qrtext::core::ast::Node> &node) override;
	qrtext::core::Associativity associativity(const QSharedPointer<qrtext::core::ast::Node> &node) override;

private:
	void returnPrecedence(int precedence);
	void returnAssociativity(qrtext::core::Associativity associativity);

	void visit(const QSharedPointer<qrtext::lua::ast::UnaryMinus> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Not> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseNegation> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::LogicalAnd> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::LogicalOr> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Addition> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Subtraction> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Multiplication> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Division> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::IntegerDivision> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Modulo> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Exponentiation> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseAnd> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseOr> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseXor> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseLeftShift> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseRightShift> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Concatenation> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Equality> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::LessThan> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::LessOrEqual> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Inequality> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::GreaterThan> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::GreaterOrEqual> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;

	int mLastRequestPresedence;
	qrtext::core::Associativity mLastRequestAssociativity;
};

}
}
