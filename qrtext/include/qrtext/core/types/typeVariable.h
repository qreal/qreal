#pragma once

#include <QtCore/QSet>
#include <QtCore/QSharedPointer>

#include "qrtext/core/types/typeExpression.h"
#include "qrtext/core/semantics/generalizationsTableInterface.h"

namespace qrtext {
namespace core {
namespace types {

class QRTEXT_EXPORT TypeVariable : public TypeExpression {
public:
	TypeVariable();
	TypeVariable(QSharedPointer<types::TypeExpression> const &singleType);

	bool isResolved() const;

	bool isEmpty() const;

	QSharedPointer<types::TypeExpression> finalType() const;

	void constrain(QSharedPointer<TypeVariable> const &other
			, GeneralizationsTableInterface const &generalizationsTable);

	void constrain(QList<QSharedPointer<TypeExpression>> const &types
			, GeneralizationsTableInterface const &generalizationsTable);

private:
	QSet<QSharedPointer<TypeExpression>> mAllowedTypes;
};

}
}
}
