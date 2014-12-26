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
	void accept(AstVisitorInterface &visitor) const override
	{
		visitor.visit(*this);
	}

	QSharedPointer<Node> mLeftOperand;
	QSharedPointer<Node> mRightOperand;
};

}
}
}
