#pragma once

#include <QtCore/QScopedPointer>

#include <qrtext/core/parser/parser.h>
#include <qrtext/core/parser/parserRef.h>
#include <qrtext/core/parser/operators/parserCombinators.h>

#include "tokenTypes.h"

namespace simpleParser {
class Parser : public qrtext::core::Parser<TokenTypes>
{
public:
	explicit Parser(QList<qrtext::core::Error> &errors);

private:
	QSharedPointer<qrtext::core::ParserInterface<TokenTypes>> grammar();
};

}
