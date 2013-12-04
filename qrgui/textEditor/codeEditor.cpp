#include "codeEditor.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

using namespace qReal;
using namespace gui;

CodeEditor::CodeEditor(QWidget *parent): QMainWindow(parent)
{
	setCentralWidget(&mCodeArea);
}

CodeEditor::CodeEditor(QString const &filename, QWidget *parent): QMainWindow(parent)
{
	setCentralWidget(&mCodeArea);
	QFile file(filename);
	QTextStream *inStream = 0;
	if (!file.isOpen() && file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		inStream = new QTextStream(&file);
	}

	if (inStream) {
		mCodeArea.setText(inStream->readAll());
	}
}

void CodeEditor::setHighlightedLineNumbers(const QList<int>& lineNumbers)
{
	Q_UNUSED(lineNumbers);
}
