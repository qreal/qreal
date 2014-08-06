#pragma once

#include <QtCore/QSharedPointer>

#include "textLanguageParser/ast/unaryOperator.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT BitwiseNegation : public UnaryOperator {
public:
	BitwiseNegation()
	{
	}
};

}
}
