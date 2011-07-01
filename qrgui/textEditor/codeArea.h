#pragma once

#include <QPlainTextEdit>
#include <QObject>
#include "textHighlighter.h"

class CodeArea : public QPlainTextEdit {
	Q_OBJECT

	public:
		CodeArea(QWidget *parent = 0);
		~CodeArea();

		void highlight(const QList<int>& lineNumbers);

	private slots:
		void highlightCurrentLine();

	private:
		TextHighlighter *mHighlighter;
};
