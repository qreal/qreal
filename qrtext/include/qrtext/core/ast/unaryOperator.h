#pragma once

#include "qrtext/core/ast/expression.h"

namespace qrtext {
namespace core {
namespace ast {

class QRTEXT_EXPORT UnaryOperator : public Expression {
public:
	QSharedPointer<Node> operand() const {
		return mOperand;
	}

	void setOperand(QSharedPointer<Node> operand) {
		mOperand = operand;
		connect(operand);
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return {mOperand};
	}

private:
	QSharedPointer<Node> mOperand;
};

}
}
}
