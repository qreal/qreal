#pragma once

#include "qrtext/core/ast/expression.h"
#include "qrtext/lua/ast/fieldInitialization.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT TableConstructor : public core::ast::Expression {
public:
	TableConstructor(QList<QSharedPointer<FieldInitialization>> const &initializers)
		: mInitializers(initializers)
	{
	}

	QList<QSharedPointer<FieldInitialization>> const &initializers() const
	{
		return mInitializers;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return as<core::ast::Node>(mInitializers);
	}

private:
	QList<QSharedPointer<FieldInitialization>> mInitializers;
};

}
}
}
