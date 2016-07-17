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

#include "qrtext/core/ast/expression.h"

namespace qrtext {
namespace core {
namespace ast {

/// Represents unary operator.
class UnaryOperator : public Expression
{
public:
	/// Returns operand.
	QSharedPointer<Node> operand() const {
		return mOperand;
	}

	/// Sets operator.
	void setOperand(QSharedPointer<Node> operand) {
		mOperand = operand;
		connect(operand);
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return {mOperand};
	}

private:
	void accept(AstVisitorInterface &visitor, const QSharedPointer<Node> &pointer
			, const QSharedPointer<Node> &parent) override
	{
		visitor.visit(qSharedPointerCast<UnaryOperator>(pointer), parent);
	}

	QSharedPointer<Node> mOperand;
};

}
}
}
