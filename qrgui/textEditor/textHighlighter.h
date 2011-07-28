#pragma once

#include <QSyntaxHighlighter>

class TextHighlighter : QSyntaxHighlighter {
	public:
		TextHighlighter(QTextDocument *document);
		void highlightBlock(const QString& text);
private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};
	QList<HighlightingRule> mHighlightingRules;

	QRegExp mCommentStartExpression;
	QRegExp mCommentEndExpression;

	QTextCharFormat mKeywordFormat;
	QTextCharFormat mClassFormat;
	QTextCharFormat mSingleLineCommentFormat;
	QTextCharFormat mMultiLineCommentFormat;
	QTextCharFormat mQuotationFormat;
	QTextCharFormat mFunctionFormat;
	QTextCharFormat mFigitsFormat;
};
