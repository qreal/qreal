#pragma once

#include "qrutils/textLanguageParser/ast/nodes/number.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT FloatNumber : public Number {
public:
	FloatNumber(QString const &stringRepresentation)
		: Number(stringRepresentation)
	{
	}
};

}
}
