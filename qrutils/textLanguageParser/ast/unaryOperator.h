#pragma once

#include <QtCore/QSharedPointer>

#include "node.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT UnaryOperator : public Node {
public:
	enum class Type {
		minus
		, notOperator
		, sharp
		, tilda
	};

	UnaryOperator(Type type, QSharedPointer<Node> operand)
		: mType(type), mOperand(operand)
	{
	}

	QSharedPointer<Node> operand() const {
		return mOperand;
	}

	Type type() const {
		return mType;
	}

private:
	Type mType;
	QSharedPointer<Node> mOperand;
};

}
}
