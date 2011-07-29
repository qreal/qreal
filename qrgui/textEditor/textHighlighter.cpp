#include "textHighlighter.h"

TextHighlighter::TextHighlighter(QTextDocument *document): QSyntaxHighlighter(document)
{
	HighlightingRule rule;

	mKeywordFormat.setForeground(Qt::darkBlue);
	mKeywordFormat.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
	keywordPatterns << "\\bconst\\b" << "\\bdouble\\b" << "\\bint\\b" << "\\bvoid\\b"
					<< "\\bwhile\\b" << "\\bif\\b" << "\\belse\\b"
					<< "\\breturn\\b" << "\\bcontinue\\b" << "\\bbreak\\b"  ;
	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = mKeywordFormat;
		mHighlightingRules.append(rule);
	}

	mClassFormat.setFontWeight(QFont::Bold);
	mClassFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
	rule.format = mClassFormat;
	mHighlightingRules.append(rule);

	mQuotationFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("\".*\"");
	rule.format = mQuotationFormat;
	mHighlightingRules.append(rule);

	mFunctionFormat.setForeground(Qt::black);
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = mFunctionFormat;
	mHighlightingRules.append(rule);

	mFunctionFormat.setForeground(Qt::blue);
	rule.pattern = QRegExp("\\b[0-9]+");
	rule.format = mFunctionFormat;
	mHighlightingRules.append(rule);

	mSingleLineCommentFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("//[^\n]*");
	rule.format = mSingleLineCommentFormat;
	mHighlightingRules.append(rule);

	mMultiLineCommentFormat.setForeground(Qt::darkGreen);

	mCommentStartExpression = QRegExp("/\\*");
	mCommentEndExpression = QRegExp("\\*/");
}

void TextHighlighter::highlightBlock(const QString& text)
{
	foreach (const HighlightingRule &rule, mHighlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}

	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = mCommentStartExpression.indexIn(text);

	while (startIndex >= 0) {
		int endIndex = mCommentEndExpression.indexIn(text, startIndex);
		int commentLength;
		if (endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		} else {
			commentLength = endIndex - startIndex + mCommentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, mMultiLineCommentFormat);
		startIndex = mCommentStartExpression.indexIn(text, startIndex + commentLength);
	}

}
