#pragma once

#include <QtScript/QScriptEngine>

#include "mainwindow/mainWindow.h"
#include "virtualCursor.h"
#include "hintReporter.h"
#include "mainwindow/palette/paletteElement.h"
#include "toolPluginInterface/usedInterfaces/hintInterface.h"


namespace qReal {
namespace gui {

class ScriptAPI : public QObject, public HintInterface{
	Q_OBJECT

public:
	ScriptAPI(MainWindow *mainWindow);

	Q_INVOKABLE void addHint(QString const &message); ///Message in error reporter
	Q_INVOKABLE void arrowToWidget(QWidget *target, qreal angle); ///Draw arrow near target
	Q_INVOKABLE void moveVirtualCursor(int xcoord, int ycoord);
	Q_INVOKABLE void dragPaletteElement();
	Q_INVOKABLE void createBlockOnScene();

private:
	QScriptEngine mScriptEngine;
	HintReporter *mHintReporter;
	MainWindow *mMainWindow;
	VirtualCursor *mVirtualCursor;
	QPropertyAnimation *animation;
};

}
}
