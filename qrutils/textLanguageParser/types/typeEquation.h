#pragma once

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include "textLanguageParser/types/typeExpression.h"
#include "textLanguageParser/ast/nodes/node.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {
namespace types {

enum class TypeRelation {
	equal
	, assignable
	, convertable
};

class QRUTILS_EXPORT TypeEquation {
public:
	TypeEquation(QSharedPointer<ast::Node> const &left, QSharedPointer<ast::Node> const &right, TypeRelation relation)
		: mLeft(left), mRight(right), mRelation(relation)
	{
	}

	QSharedPointer<ast::Node> const &left() const
	{
		return mLeft;
	}

	QSharedPointer<ast::Node> const &right() const
	{
		return mRight;
	}

	TypeRelation const &relation() const
	{
		return mRelation;
	}

private:
	QSharedPointer<ast::Node> mLeft;
	QSharedPointer<ast::Node> mRight;
	TypeRelation mRelation;
};

}
}
