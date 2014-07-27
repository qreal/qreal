#pragma once

namespace textLanguageParser {

class ParserError {
public:
	enum ErrorType {
		lexicalError
		, syntaxError
		, semanticError
	};
};

}
