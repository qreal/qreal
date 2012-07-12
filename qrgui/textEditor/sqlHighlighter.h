#pragma once
#include <QSyntaxHighlighter>

class SQLHighlighter : public QSyntaxHighlighter
{
public:
	SQLHighlighter(QTextDocument *document);
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
	QTextCharFormat mDigitalFormat;
	QTextCharFormat mStringFormat;
	QTextCharFormat mSingleLineCommentFormat;
	QTextCharFormat mMultiLineCommentFormat;
	QTextCharFormat mSignFormat;
	QTextCharFormat mSomeStringFormat;
};
