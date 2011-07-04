#pragma once

#include <QMainWindow>
#include "codeArea.h"

class CodeEditor : public QMainWindow {
	Q_OBJECT

	public:
		CodeEditor(QWidget *parent = 0);
		CodeEditor(const QString& filename, QWidget *parent = 0);

		void setHighlightedLineNumbers(const QList<int>& lineNumbers);
	
	private:
		CodeArea mCodeArea;
};
