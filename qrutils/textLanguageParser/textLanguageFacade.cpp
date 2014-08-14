#include "textLanguageParser/textLanguageFacade.h"

using namespace textLanguageParser;

TextLanguageFacade::TextLanguageFacade()
	: mAnalyzer(mErrors), mInterpreter(mErrors)
{
}

void TextLanguageFacade::markAsChanged(qReal::Id const &id, QString const &propertyName)
{
	Q_UNUSED(id)
	Q_UNUSED(propertyName)
}

QSharedPointer<ast::Node> const &TextLanguageFacade::parse(qReal::Id const &id
		, QString const &propertyName, QString const &code)
{
	auto result = mParser.parse(code);
	mErrors << result.errors;
	mAstRoots[id][propertyName] = result.astRoot;

	mAnalyzer.analyze(result.astRoot);

	return mAstRoots[id][propertyName];
}

QSharedPointer<ast::Node> TextLanguageFacade::ast(qReal::Id const &id, QString const &propertyName) const
{
	return mAstRoots[id][propertyName];
}

QSharedPointer<types::TypeExpression> TextLanguageFacade::type(QSharedPointer<ast::Node> const &expression) const
{
	return mAnalyzer.type(expression);
}

QList<ParserError> const &TextLanguageFacade::errors() const
{
	return mErrors;
}
