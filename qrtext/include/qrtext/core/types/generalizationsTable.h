#pragma once

#include "qrutils/textLanguageParser/types/typeExpression.h"

namespace textLanguageParser {
namespace types {

class QRUTILS_EXPORT GeneralizationsTable {
public:
	GeneralizationsTable();

	bool isGeneralization(QSharedPointer<TypeExpression> const &specific
			, QSharedPointer<TypeExpression> const &general) const;

private:
	bool isStructurallyEquivalent(QSharedPointer<TypeExpression> const &type1
			, QSharedPointer<TypeExpression> const &type2) const;
};

}
}
