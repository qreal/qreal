#pragma once

#include <QtCore/QScopedPointer>

#include "qrtext/core/semantics/generalizationsTableInterface.h"

namespace qrtext {
namespace lua {
namespace details {

class LuaGeneralizationsTable : public core::GeneralizationsTableInterface {
public:
	bool isGeneralization(QSharedPointer<core::types::TypeExpression> const &specific
			, QSharedPointer<core::types::TypeExpression> const &general) const override;

private:
	bool isStructurallyEquivalent(QSharedPointer<core::types::TypeExpression> const &type1
			, QSharedPointer<core::types::TypeExpression> const &type2) const;
};

}
}
}
