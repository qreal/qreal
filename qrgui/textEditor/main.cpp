#include <QtGui>

#include "codeEditor.h"

int main(int argv, char **args) {
	QApplication app(argv, args);

	CodeEditor editor("1.cpp");
	editor.setWindowTitle(QObject::tr("Code Editor Example"));

	QList<int> a;
	a.append(1);
	a.append(3);
	a.append(5);
	editor.setHighlightedLineNumbers(a);

	editor.show();

	return app.exec();
}

