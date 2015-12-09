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

/// Represents binary operator, that can have left and right operand.
class BinaryOperator : public Expression
{
public:
	/// Sets left operand, replaces existing.
	void setLeftOperand(QSharedPointer<Node> const &leftOperand)
	{
		mLeftOperand = leftOperand;
		connect(leftOperand);
	}

	/// Sets right operand, replaces existing.
	void setRightOperand(QSharedPointer<Node> const &rightOperand)
	{
		mRightOperand = rightOperand;
		connect(rightOperand);
	}

	/// Returns left operand.
	QSharedPointer<Node> const &leftOperand() const
	{
		return mLeftOperand;
	}

	/// Returns right operand.
	QSharedPointer<Node> const &rightOperand() const
	{
		return mRightOperand;
	}

	/// Returns left and right operand as a list.
	QList<QSharedPointer<Node>> children() const override
	{
		return {mLeftOperand, mRightOperand};
	}

private:
	void accept(AstVisitorInterface &visitor, const QSharedPointer<Node> &pointer
			, const QSharedPointer<Node> &parent) override
	{
		visitor.visit(qSharedPointerCast<BinaryOperator>(pointer), parent);
	}

	QSharedPointer<Node> mLeftOperand;
	QSharedPointer<Node> mRightOperand;
};

}
}
}
