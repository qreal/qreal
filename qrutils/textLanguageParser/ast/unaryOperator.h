#pragma once

#include <QtCore/QSharedPointer>

#include "node.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT UnaryOperator : public Node {
public:
	UnaryOperator()
	{
	}

	QSharedPointer<Node> operand() const {
		return mOperand;
	}

	void setOperand(QSharedPointer<Node> operand) {
		mOperand = operand;
		connect(*operand);
	}

private:
	QSharedPointer<Node> mOperand;
};

}
}
