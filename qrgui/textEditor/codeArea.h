#pragma once

#include <QPlainTextEdit>
#include <QObject>
#include "textHighlighter.h"

class CodeArea : public QPlainTextEdit {
	Q_OBJECT

	public:
		CodeArea(QWidget *parent = 0);
		~CodeArea();

		void setHighlightedLineNumbers(const QList<int>&);

	private slots:
		void highlightCurrentLine();

	private:
		QList<QTextEdit::ExtraSelection> highlightedLinesSelectionList();

		TextHighlighter *mHighlighter;
		QList<int> mHighlightedLineNumbers;
};
