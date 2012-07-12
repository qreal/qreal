#pragma once

#include <QSyntaxHighlighter>
#include <QPlainTextEdit>
#include <QObject>

enum highlighterType {
	text,
	sql
};

class CodeArea : public QPlainTextEdit {
	Q_OBJECT

	public:
		CodeArea(QWidget *parent = 0, highlighterType type = text);
		~CodeArea();

		void setHighlightedLineNumbers(const QList<int>&);

	private slots:
		void highlightCurrentLine();

	private:
		QList<QTextEdit::ExtraSelection> highlightedLinesSelectionList();

		QSyntaxHighlighter *mHighlighter;
		QList<int> mHighlightedLineNumbers;
};
