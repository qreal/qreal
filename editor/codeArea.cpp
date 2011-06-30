#include <codeArea.h>

CodeArea::CodeArea(QWidget *parent): QPlainTextEdit(parent) {
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	highlighter = new TextHighlighter(document());

	highlightCurrentLine();
}

CodeArea::~CodeArea() {
	if (highlighter)
		delete highlighter;
}

void CodeArea::highlightCurrentLine() {
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly()) {
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(Qt::blue).lighter(160);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}
