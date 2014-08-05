#pragma once

#include "node.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT Number : public Node {
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
