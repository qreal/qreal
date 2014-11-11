#pragma once

#include "qrtext/core/types/typeExpression.h"

namespace qrtext {
namespace lua {
namespace types {

/// Represents table type.
class Table : public core::types::TypeExpression
{
public:
	/// Constructor.
	/// @param elementType - type of table element.
	/// @param size - size of a table.
	Table(QSharedPointer<core::types::TypeExpression> const &elementType
			, int size)
		: mElementType(elementType), mSize(size)
	{
	}

	/// Returns type of table element.
	QSharedPointer<core::types::TypeExpression> const &elementType() const
	{
		return mElementType;
	}

	QString toString() const override
	{
		return QObject::tr("table[%1]").arg(mElementType->toString());
	}

private:
	QSharedPointer<core::types::TypeExpression> mElementType;
	int mSize;
};

}
}
}
