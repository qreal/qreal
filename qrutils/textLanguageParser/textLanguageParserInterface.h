#pragma once

#include "ast/node.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {

/// Client interface for QReal text language parser.
class TextLanguageParserInterface {
	virtual ~TextLanguageParserInterface() {}

	/// Parses given code and returns AST with results or nullptr if parsing is impossible.
	virtual Node *parse(QString const &code) = 0;
};

}
