#include "ruleHighlighter.h"
#include "keywords.h"

using namespace generationRules;

RuleHighlighter::RuleHighlighter(QTextDocument *parent)
	: QSyntaxHighlighter(parent)
{
	HighlightingRule rule;

	mKeywordFormat.setForeground(Qt::darkBlue);
	mKeywordFormat.setFontWeight(QFont::Bold);

	QStringList keywordPatterns;
	keywordPatterns << listWithSpecialSymbols(keywords::generalTemplates)
			<< listWithSpecialSymbols(keywords::textTemplates)
			<< listWithSpecialSymbols(keywords::linksTemplates);

	for (const auto &pattern : keywordPatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = mKeywordFormat;

		mHighlightingRules << rule;
	}

	mConstantTextFormat.setForeground(Qt::red);
	rule.pattern = QRegExp(R"(('[^'\\]*(\\(.|\n)[^'\\]*)*'))");
	rule.format = mConstantTextFormat;
	mHighlightingRules.append(rule);
}

void RuleHighlighter::highlightBlock(const QString &text)
{
	for (const auto &rule : mHighlightingRules) {
		QRegExp expression(rule.pattern);
		auto index = expression.indexIn(text);

		while (index >= 0) {
			const auto length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}
}

QStringList RuleHighlighter::listWithSpecialSymbols(const QStringList &listToAppendSymbols)
{
	QStringList result;

	for (const auto &listElement : listToAppendSymbols) {
		const auto newElement = "\\b" + listElement + "\\b";
		result << newElement;
	}

	return result;
}
