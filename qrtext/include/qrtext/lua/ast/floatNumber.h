#pragma once

#include "qrtext/lua/ast/number.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT FloatNumber : public Number {
public:
	FloatNumber(QString const &stringRepresentation)
		: Number(stringRepresentation)
	{
	}
};

}
}
}
