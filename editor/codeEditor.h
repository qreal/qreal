#pragma once

#include <QMainWindow>
#include "codeArea.h"

class CodeEditor : public QMainWindow {
	Q_OBJECT

	public:
		CodeEditor(QWidget *parent = 0);
	
	private:
		CodeArea codeArea;
};
