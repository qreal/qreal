#pragma once

#include "qrtext/lua/ast/expression.h"
#include "qrtext/lua/ast/fieldInitialization.h"

namespace qrtext {
namespace lua {
namespace ast {

/// Represents table constructor expression.
class QRTEXT_EXPORT TableConstructor : public Expression
{
public:
	/// Constructor.
	/// @param initializers - a list of field initializers.
	TableConstructor(QList<QSharedPointer<FieldInitialization>> const &initializers)
		: mInitializers(initializers)
	{
	}

	/// Returns a list of field initializers.
	QList<QSharedPointer<FieldInitialization>> const &initializers() const
	{
		return mInitializers;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return as<Node>(mInitializers);
	}

private:
	QList<QSharedPointer<FieldInitialization>> mInitializers;
};

}
}
}
