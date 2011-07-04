#pragma once

#include <QSyntaxHighlighter>

class TextHighlighter : QSyntaxHighlighter {
	public:
		TextHighlighter(QTextDocument *document);
		void highlightBlock(const QString& text);
};
