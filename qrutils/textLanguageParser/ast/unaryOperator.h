#pragma once

#include "expression.h"

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

private:
	QSharedPointer<Node> mOperand;
};

}
}
