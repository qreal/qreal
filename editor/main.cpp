#include <QtGui>

#include "codeEditor.h"

int main(int argv, char **args) {
    QApplication app(argv, args);

    CodeEditor editor;
    editor.setWindowTitle(QObject::tr("Code Editor Example"));
    editor.show();

    return app.exec();
}

