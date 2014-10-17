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
	void accept(AstVisitorInterface &visitor) const override
	{
		visitor.visit(*this);
	}

	QSharedPointer<Node> mOperand;
};

}
}
}
