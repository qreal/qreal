#pragma once

#include <QtCore/QSet>
#include <QtCore/QSharedPointer>

#include "qrutils/textLanguageParser/types/typeExpression.h"
#include "qrutils/textLanguageParser/types/generalizationsTable.h"

namespace textLanguageParser {
namespace types {

class QRUTILS_EXPORT TypeVariable : public TypeExpression {
public:
	TypeVariable();
	TypeVariable(QSharedPointer<types::TypeExpression> const &singleType);

	bool isResolved() const;

	bool isEmpty() const;

	QSharedPointer<types::TypeExpression> finalType() const;

	void constrain(QSharedPointer<TypeVariable> const &other, GeneralizationsTable const &generalizationsTable);

	void constrain(QList<QSharedPointer<TypeExpression>> const &types
			, GeneralizationsTable const &generalizationsTable);

private:
	QSet<QSharedPointer<TypeExpression>> mAllowedTypes;
};

}
}
