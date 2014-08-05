#pragma once

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

	UnaryOperator(Type type)
		: mType(type)
	{
	}

	void setOperand(Node *operand) {
		mOperand = operand;
	}

	Node *operand() const {
		return mOperand;
	}

	Type type() const {
		return mType;
	}

private:
	Type mType;
	Node *mOperand;
};

}
}
