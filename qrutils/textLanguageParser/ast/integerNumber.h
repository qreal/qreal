#pragma once

#include "textLanguageParser/ast/number.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT IntegerNumber : public Number {
public:
	IntegerNumber(QString const &stringRepresentation)
		: Number(stringRepresentation)
	{
	}
};

}
}
