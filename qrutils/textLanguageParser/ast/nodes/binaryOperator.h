#pragma once

#include "textLanguageParser/ast/nodes/expression.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT BinaryOperator : public Expression
{
public:
	void setLeftOperand(QSharedPointer<Node> leftOperand)
	{
		mLeftOperand = leftOperand;
		connect(leftOperand);
	}

	void setRightOperand(QSharedPointer<Node> rightOperand)
	{
		mRightOperand = rightOperand;
		connect(rightOperand);
	}

	QSharedPointer<Node> leftOperand() const
	{
		return mLeftOperand;
	}

	QSharedPointer<Node> rightOperand() const
	{
		return mRightOperand;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return {mLeftOperand, mRightOperand};
	}

private:
	QSharedPointer<Node> mLeftOperand;
	QSharedPointer<Node> mRightOperand;
};

}
}
