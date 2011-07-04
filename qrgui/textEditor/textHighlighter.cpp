#include "textHighlighter.h"

TextHighlighter::TextHighlighter(QTextDocument *document): QSyntaxHighlighter(document) {
}

void TextHighlighter::highlightBlock(const QString& text) {
	QTextCharFormat myClassFormat;
	myClassFormat.setFontWeight(QFont::Bold);
	myClassFormat.setForeground(Qt::darkGreen);
	//QString pattern = "\\bMy[A-Za-z]+\\b";
	QString pattern = "\\b(class|void|int|double|long|virtual)\\b";

	QRegExp expression(pattern);
	int index = text.indexOf(expression);
	while (index >= 0) {
		int length = expression.matchedLength();
		setFormat(index, length, myClassFormat);
		index = text.indexOf(expression, index + length);
	}
}
