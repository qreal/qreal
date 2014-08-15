#pragma once

#include "qrutils/textLanguageParser/ast/nodes/node.h"
#include "qrutils/textLanguageParser/types/typeExpression.h"
#include "qrutils/textLanguageParser/parserError.h"

#include "qrutils/textLanguageParser/textLanguageParser.h"
#include "qrutils/textLanguageParser/semanticAnalyzer.h"
#include "qrutils/textLanguageParser/interpreter.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {

/// Facade that uses text language parsing/interpreting framework to provide services based on "standard" text
/// language (Lua 5.3 subset).
///
/// Facade can be used for interpreting diagrams or for syntax analysis in generators. It allows to pass code fragments
/// as chunks (corresponding to Lua blocks) identified by Id of a node and name of the property where this code fragment
/// was taken from. It can interpret it immediately, returning result and storing syntax tree and interpreter state for
/// future uses, or can parse it and then return AST for node/property by request (note that it uses all ASTs
/// as a context for parsing next chunks).
///
/// Note that types of variables may change during parsing next chunks, so, for example, after parsing "a = 123" type
/// of "a" will be inferred as Integer, but after parsing "a = 1.0" it will be changed to Float. Generators may reliably
/// use type information only when all code in a program is parsed.
class QRUTILS_EXPORT TextLanguageFacade {
public:
	TextLanguageFacade();

	template<typename T>
	T interpret(qReal::Id const &id, QString const &propertyName, QString const &code)
	{
		auto root = parse(id, propertyName, code);
		return mInterpreter.interpret<T>(root, mAnalyzer);
	}

	void markAsChanged(qReal::Id const &id, QString const &propertyName);

	QSharedPointer<ast::Node> const &parse(qReal::Id const &id, QString const &propertyName, QString const &code);

	QSharedPointer<ast::Node> ast(qReal::Id const &id, QString const &propertyName) const;

	QSharedPointer<types::TypeExpression> type(QSharedPointer<ast::Node> const &expression) const;

	QList<ParserError> const &errors() const;

private:
	QList<ParserError> mErrors;

	TextLanguageParser mParser;
	SemanticAnalyzer mAnalyzer;
	Interpreter mInterpreter;

	QHash<qReal::Id, QHash<QString, QSharedPointer<ast::Node>>> mAstRoots;
};

}
