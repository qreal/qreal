#pragma once

#include "textLanguageParser/ast/nodes/expression.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT UnaryOperator : public Expression {
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
