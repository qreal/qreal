#pragma once

#include <QtCore/QList>

#include "ast/node.h"
#include "parserError.h"

namespace textLanguageParser {

/// Client interface for QReal text language parser.
class TextLanguageParserInterface {
public:

	/// Result of parsing.
	struct Result {
		/// Root of constructed AST, or nullptr if parsing failed.
		ast::Node * astRoot;

		/// A list of detected lexer errors.
		QList<ParserError> errors;

		Result(ast::Node * const astRoot, QList<ParserError> errors)
			: astRoot(astRoot), errors(errors)
		{
		}
	};

	virtual ~TextLanguageParserInterface() {}

	/// Parses given code and returns AST with results and a list of errors. Passes ownership to caller.
	virtual Result parse(QString const &code) = 0;
};

}
