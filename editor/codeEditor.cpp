#include <codeEditor.h>

CodeEditor::CodeEditor(QWidget *parent): QMainWindow(parent), codeArea(this) {
	setCentralWidget(&codeArea);
}
