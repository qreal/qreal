#pragma once

#include "expression.h"
#include "fieldInitialization.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT TableConstructor : public Expression {
public:
	TableConstructor(QList<QSharedPointer<FieldInitialization>> const &initializers)
		: mInitializers(initializers)
	{
	}

	QList<QSharedPointer<FieldInitialization>> const &initializers() const
	{
		return mInitializers;
	}

private:
	QList<QSharedPointer<FieldInitialization>> mInitializers;
};

}
}
