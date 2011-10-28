#include <QDebug>
#include <QtAlgorithms>
#include "codeArea.h"

CodeArea::CodeArea(QWidget *parent): QPlainTextEdit(parent)
{
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	mHighlighter = new TextHighlighter(document());

	highlightCurrentLine();
	setReadOnly(true);
}

CodeArea::~CodeArea()
{
	if (mHighlighter)
		delete mHighlighter;
}

void CodeArea::highlightCurrentLine()
{
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

	extraSelections.append(highlightedLinesSelectionList());
	setExtraSelections(extraSelections);
}

void CodeArea::setHighlightedLineNumbers(const QList<int>& list)
{
	mHighlightedLineNumbers = list;
}

QList<QTextEdit::ExtraSelection> CodeArea::highlightedLinesSelectionList()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly()) {
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(Qt::red).lighter(160);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);

		foreach (int lineNum, mHighlightedLineNumbers) {
			selection.cursor = QTextCursor(document()->findBlockByNumber(lineNum));
			selection.cursor.clearSelection();
			//selection.cursor.select(QTextCursor::LineUnderCursor);
			selection.cursor.select(QTextCursor::BlockUnderCursor);
			extraSelections.append(selection);
		}
	}

	return extraSelections;
}
