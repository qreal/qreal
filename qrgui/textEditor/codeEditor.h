#pragma once

#include <QtWidgets/QMainWindow>

#include "textEditor/codeArea.h"

namespace qReal {
namespace gui {

class CodeEditor : public QMainWindow {
	Q_OBJECT

	public:
		CodeEditor(QWidget *parent = 0);
		CodeEditor(QString const &filename, QWidget *parent = 0);

		void setHighlightedLineNumbers(const QList<int>& lineNumbers);

	private:
		CodeArea mCodeArea;
};

}
}
