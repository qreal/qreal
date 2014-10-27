#pragma once

#include <QtGui/QSyntaxHighlighter>

/// Syntax Highlighter for sql code.
class SQLHighlighter : public QSyntaxHighlighter
{
public:
	SQLHighlighter(QTextDocument *document);
	void highlightBlock(QString const &text);

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
	QTextCharFormat mDigitalFormat;
	QTextCharFormat mStringFormat;
	QTextCharFormat mSingleLineCommentFormat;
	QTextCharFormat mMultiLineCommentFormat;
	QTextCharFormat mSignFormat;
	QTextCharFormat mSomeStringFormat;
};
