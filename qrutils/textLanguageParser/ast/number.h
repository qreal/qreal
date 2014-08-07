#pragma once

#include "expression.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT Number : public Expression {
public:
	Number(QString const &stringRepresentation)
		: mStringRepresentation(stringRepresentation)
	{
	}

	QString const &stringRepresentation() const {
		return mStringRepresentation;
	}

private:
	QString mStringRepresentation;
};

}
}
