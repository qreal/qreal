#pragma once

#include <QtGui/QSyntaxHighlighter>

namespace generationRules {

/// Class for highlighting generation rules.
class RuleHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param parent - text document we want to highlight.
	RuleHighlighter(QTextDocument *parent = 0);

protected:
	void highlightBlock(const QString &text);

private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};

	static QStringList listWithSpecialSymbols(const QStringList listToAppendSymbols);

	QVector<HighlightingRule> mHighlightingRules;

	QTextCharFormat mKeywordFormat;
	QTextCharFormat mConstantTextFormat;
};

}
