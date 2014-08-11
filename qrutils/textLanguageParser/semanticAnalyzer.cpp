#include "textLanguageParser/semanticAnalyzer.h"

using namespace textLanguageParser;

SemanticAnalyzer::SemanticAnalyzer()
{
}

void SemanticAnalyzer::analyze(QSharedPointer<ast::Node> const &root, QList<ParserError> &errors)
{
	if (!root->is<ast::Expression>()) {
		for (auto child : root->children()) {
			analyze(child, errors);
		}
	}
}

types::TypeExpression const &SemanticAnalyzer::type(QSharedPointer<ast::Expression> const &expression) const
{
}
