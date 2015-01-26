#pragma once

#include <QtGui/QSyntaxHighlighter>

namespace qReal {
namespace text {

class TextHighlighter : public QSyntaxHighlighter
{
public:
	TextHighlighter(QTextDocument *document);
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
	QTextCharFormat mClassFormat;
	QTextCharFormat mSingleLineCommentFormat;
	QTextCharFormat mMultiLineCommentFormat;
	QTextCharFormat mQuotationFormat;
	QTextCharFormat mFunctionFormat;
	QTextCharFormat mFigitsFormat;
};

}
}
