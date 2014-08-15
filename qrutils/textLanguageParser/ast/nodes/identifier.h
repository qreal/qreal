#pragma once

#include "qrutils/textLanguageParser/ast/nodes/expression.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT Identifier : public Expression {
public:
	Identifier(QString const &name)
		: mName(name)
	{
	}

	QString const &name() const {
		return mName;
	}

private:
	QString mName;
};

}
}
