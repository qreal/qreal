#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "codeEditor.h"

using namespace qReal;
using namespace gui;

CodeEditor::CodeEditor(QWidget *parent): QMainWindow(parent), mCodeArea(this)
{
	setCentralWidget(&mCodeArea);
}

CodeEditor::CodeEditor(const QString& filename, QWidget *parent): QMainWindow(parent), mCodeArea(this) {
	setCentralWidget(&mCodeArea);
	QFile file(filename);
	QTextStream *inStream = 0;
	if (!file.isOpen() && file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		inStream = new QTextStream(&file);
	}

	if (inStream) {
		mCodeArea.document()->setPlainText(inStream->readAll());
	}
}

void CodeEditor::setHighlightedLineNumbers(const QList<int>& lineNumbers)
{
	mCodeArea.setHighlightedLineNumbers(lineNumbers);
}
