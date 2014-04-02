#pragma once

#include <QtScript/QScriptEngine>

#include "mainwindow/hint.h"
#include "mainwindow/errorReporter.h"
#include "toolPluginInterface/usedInterfaces/hintInterface.h"


namespace qReal {
namespace gui {

class ErrorReporter;

class ScriptAPI : public QObject, public HintInterface{
	Q_OBJECT

public:
	ScriptAPI(ErrorReporter *errorReporter, MainWindow *mainWindow);

	Q_INVOKABLE void addHint(QString const &message); ///Message in error reporter
	Q_INVOKABLE void arrowToWidget(QWidget *target, qreal angle); ///Draw arrow near target

private:
	QScriptEngine mScriptEngine;
	ErrorReporter *mErrorReporter;
	MainWindow *mMainWindow;
};

}
}
