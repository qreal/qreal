#pragma once

#include "qrtext/core/types/typeExpression.h"

namespace qrtext {
namespace core {

class GeneralizationsTableInterface {
public:
	virtual ~GeneralizationsTableInterface() {}

	virtual bool isGeneralization(QSharedPointer<types::TypeExpression> const &specific
			, QSharedPointer<types::TypeExpression> const &general) const = 0;
};

}
}
