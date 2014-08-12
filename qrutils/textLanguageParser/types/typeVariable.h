#pragma once

#include <QtCore/QSet>
#include <QtCore/QSharedPointer>

#include "textLanguageParser/types/typeExpression.h"

namespace textLanguageParser {
namespace types {

class QRUTILS_EXPORT TypeVariable : public TypeExpression {
public:
	TypeVariable();
	TypeVariable(QSharedPointer<types::TypeExpression> const &singleType);

	bool isResolved() const;

	QSharedPointer<types::TypeExpression> finalType() const;

private:
	QSet<QSharedPointer<TypeExpression>> mAllowedTypes;
};

}
}
