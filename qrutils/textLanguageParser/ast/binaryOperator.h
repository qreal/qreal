#pragma once

#include <QtCore/QSharedPointer>

#include "node.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT BinaryOperator : public Node
{
public:
	BinaryOperator()
	{
	}

	void setLeftOperand(QSharedPointer<Node> leftOperand)
	{
		mLeftOperand = leftOperand;
		connect(*leftOperand);
	}

	void setRightOperand(QSharedPointer<Node> rightOperand)
	{
		mRightOperand = rightOperand;
		connect(*rightOperand);
	}

	QSharedPointer<Node> leftOperand() const
	{
		return mLeftOperand;
	}

	QSharedPointer<Node> rightOperand() const
	{
		return mRightOperand;
	}

private:
	QSharedPointer<Node> mLeftOperand;
	QSharedPointer<Node> mRightOperand;
};

}
}
