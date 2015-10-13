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
