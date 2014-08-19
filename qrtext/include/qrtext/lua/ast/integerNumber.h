#pragma once

#include "qrtext/lua/ast/number.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT IntegerNumber : public Number {
public:
	IntegerNumber(QString const &stringRepresentation)
		: Number(stringRepresentation)
	{
	}
};

}
}
}
