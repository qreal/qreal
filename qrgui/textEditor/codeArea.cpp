#include <QDebug>
#include <QtAlgorithms>
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

		QColor lineColor = QColor(Qt::blue).lighter(160);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);

	QList<int> a;
	a.append(1);
	a.append(3);
	a.append(5);
	highlight(a);
}

void CodeArea::highlight(const QList<int>& lineNumbers) {
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly()) {
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(Qt::red).lighter(160);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);

		foreach (int lineNum, lineNumbers) {
			selection.cursor = QTextCursor(document()->findBlockByNumber(lineNum));
			selection.cursor.clearSelection();
			selection.cursor.select(QTextCursor::LineUnderCursor);
			extraSelections.append(selection);
		}
	}

	setExtraSelections(extraSelections);
}
