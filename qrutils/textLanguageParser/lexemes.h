#pragma once

#include <QtCore/QHash>
#include <QtCore/QRegularExpression>

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {

class Lexemes {
public:
	Lexemes();

	enum Type {
		whitespace
		, newline
		, integer
		, identifier
	};

	void redefine(Type lexemeType, QRegularExpression const &regExp);

	/// @todo Hide it.
	QHash<Type, QRegularExpression> const &lexemes() const;

private:
	QHash<Type, QRegularExpression> mLexemes;
};

}
