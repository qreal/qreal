#include "codeArea.h"

CodeArea::CodeArea(QWidget *parent): QPlainTextEdit(parent) {
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	mHighlighter = new TextHighlighter(document());

	highlightCurrentLine();
}

CodeArea::~CodeArea() {
	if (mHighlighter)
		delete mHighlighter;
}

void CodeArea::highlightCurrentLine() {
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly()) {
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(Qt::blue).lighter(200);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}
