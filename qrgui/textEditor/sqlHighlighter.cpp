/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "sqlHighlighter.h"

using namespace qReal;
using namespace text;

SQLHighlighter::SQLHighlighter(QTextDocument *document)
	: QSyntaxHighlighter(document)
{
	HighlightingRule rule;

	mSignFormat.setForeground(Qt::green);
	QStringList signPatterns;
	signPatterns << "," << ">" << "<" << "-" << "\\*" << "\\+"
				 << "\\." << "=" << "\\(" << "\\)";
	foreach (const QString &pattern, signPatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = mSignFormat;
		mHighlightingRules.append(rule);
	}

	mKeywordFormat.setForeground(Qt::darkRed);
	mKeywordFormat.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
	keywordPatterns << "\\bSELECT\\b" << "\\bFROM\\b" << "\\bWHERE\\b" << "\\bORDER BY\\b"
					<< "\\bCOUNT\\b" << "\\bAS\\b" << "\\bJOIN\\b" << "\\bON\\b"
					<< "\\bGROUP BY\\b" << "\\bHAVING\\b" << "\\bNATURAL JOIN\\b"  ;
	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = mKeywordFormat;
		mHighlightingRules.append(rule);
	}

	mStringFormat.setForeground(Qt::red);
	rule.pattern = QRegExp("\\'[A-Za-z0-9]+\\'");
	rule.format = mStringFormat;
	mHighlightingRules.append(rule);

	mSomeStringFormat.setForeground(Qt::black);
	rule.pattern = QRegExp("\\\"[A-Za-z0-9]+\\\"");
	rule.format = mSomeStringFormat;
	mHighlightingRules.append(rule);

	mDigitalFormat.setForeground(Qt::magenta);
	rule.pattern = QRegExp("\\b[0-9]+\\.?[0-9]+\\b");
	rule.format = mDigitalFormat;
	mHighlightingRules.append(rule);

	mSingleLineCommentFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("--[^\n]*");
	rule.format = mSingleLineCommentFormat;
	mHighlightingRules.append(rule);

	mMultiLineCommentFormat.setForeground(Qt::darkGreen);

	mCommentStartExpression = QRegExp("\\{");
	mCommentEndExpression = QRegExp("\\}");
}

void SQLHighlighter::highlightBlock(const QString &text)
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
	if (previousBlockState() != 1) {
		startIndex = mCommentStartExpression.indexIn(text);
	}

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
