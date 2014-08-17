#pragma once

#include <QtCore/QScopedPointer>

#include "qrutils/textLanguageParser/textLanguageParserInterface.h"
#include "qrutils/textLanguageParser/details/token.h"
#include "qrutils/textLanguageParser/details/tokenStream.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {

/// Parser for a subset of Lua 5.3 language. Syntax definition of a base language can be found here:
/// http://www.lua.org/work/doc/manual.html#9
/// We use modified grammar with following rationale.
/// - No statements except function calls, variable declarations and expressions. Algorithmic constructions shall be
///   drawn on a diagram instead of written. This may change in future versions, since it may be convenient to use
///   small algorithmic chunks of code inside a block.
/// - Statement can be an expression. Return statement removed, instead we use convention that each statement block has
///   value, and this value is a value of a last statement, if it is an expression, or no value otherwise. This is
///   much like F#, and needed so we can interpret constants or simple expressions as valid Lua programs.
/// - Grammar is factorized and left recursion removed to allow recursive descent. Prefix expressions and related
///   productions required a significant rework which made grammar more "wide" than required, but it will be compensated
///   by semantic actions in parser. See http://lua-users.org/lists/lua-l/2011-10/msg00442.html.
/// - Due to many precedence levels expressions are parsed by Precedence Climbing algorithm, so "exp" production is
///   parameterized by current precedence level. See http://www.engr.mun.ca/~theo/Misc/exp_parsing.htm.
/// Resulting grammar is as follows:
/// @verbatim
/// Start ::= block
///
/// block ::= {stat}
///
/// // Here we shall check that if "=" part is present, left-hand expressions are actually variables, matching
/// // "prefixexp {‘,’ prefixexp}" grammar rule, and prefix expressions inside are not ended with function calls.
/// // Lua grammar allows, for example, f(3)[0] = 42,
/// // but not f(3) = 42. Here we want to stay close to standard and check such things in semantic action.
/// // If "=" part is omitted, then if explist contains only one expression, it is parsed as itself (Expression node),
/// // if there are more than one, it becomes a table.
/// stat ::= ‘;’
///     | explist [‘=’ explist]
///
/// explist ::= exp(0) {‘,’ exp(0)}
///
/// exp(precedence) ::= primary { binop exp(newPrecedence) }
///
/// primary ::= nil | false | true | Number | String | ‘...’
///     | prefixexp | tableconstructor | unop exp(newPrecedence)
///
/// prefixexp ::= prefixterm { functioncallpart | varpart }
///
/// varpart ::= ‘[’ exp(0) ‘]’
///             | ‘.’ Name
///
/// functioncallpart :: = args
///                       | ‘:’ Name args
///
/// prefixterm ::= Name
///                | ‘(’ exp(0) ‘)’
///
/// args ::= ‘(’ [explist] ‘)’
///          | tableconstructor
///          | String
///
/// tableconstructor ::= ‘{’ [fieldlist] ‘}’
///
/// fieldlist ::= field {fieldsep field} [fieldsep]
///
/// field ::= ‘[’ exp(0) ‘]’ ‘=’ exp(0)
///           | exp(0) [ ‘=’ exp(0) ]             // Here we check that if "=" part is present, first exp is Name.
///
/// fieldsep ::= ‘,’ | ‘;’
///
/// binop ::= ‘+’ | ‘-’ | ‘*’ | ‘/’ | ‘//’ | ‘^’ | ‘%’ |
///     ‘&’ | ‘~’ | ‘|’ | ‘>>’ | ‘<<’ | ‘..’ |
///     ‘<’ | ‘<=’ | ‘>’ | ‘>=’ | ‘==’ | ‘~=’ |
///     and | or
///
/// unop ::= ‘-’ | not | ‘#’ | ‘~’
/// @endverbatim

class QRUTILS_EXPORT TextLanguageParser : public TextLanguageParserInterface {
public:
	/// Parses given code and returns AST with results or nullptr if parsing is impossible.
	Result parse(QString const &code) override;

private:
	QScopedPointer<details::TokenStream> mTokenStream;
	QList<ParserError> mErrors;
};

}
