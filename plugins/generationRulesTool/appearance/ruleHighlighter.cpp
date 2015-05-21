#include "ruleHighlighter.h"

using namespace generationRules;

RuleHighlighter::RuleHighlighter(QTextDocument *parent)
	: QSyntaxHighlighter(parent)
{
	HighlightingRule rule;

	mKeywordFormat.setForeground(Qt::darkBlue);
	mKeywordFormat.setFontWeight(QFont::Bold);

	QStringList keywordPatterns;
	keywordPatterns << "\\bforeach\\b" << "\\bin\\b" << "\\bnewline\\b"
			<< "\\bcallGeneratorFor\\b" << "\\bGenerator\\b" << "\\bthis\\b"
			<< "\\btab\\b" << "\\bgenerateToFile\\b"
			<< "\\boutcomingLinks\\b" << "\\bincomingLinks\\b" << "\\blinks\\b"
			<< "\\btransitionEnd\\b" << "\\btransitionStart\\b"
			<< "\\bif\\b" << "\\belse\\b"
			<< "\\bforeach_excludeLast\\b";

	for (const auto pattern : keywordPatterns) {
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
	for (const auto rule : mHighlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);

		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}
}
