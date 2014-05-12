#pragma once

#include <QtScript/QScriptEngine>

#include "mainwindow/mainWindow.h"
#include "virtualCursor.h"
#include "hintReporter.h"
#include "guiFacade.h"
#include "mainwindow/palette/paletteElement.h"
#include "toolPluginInterface/usedInterfaces/hintInterface.h"

namespace qReal {
namespace gui {

class ScriptAPI : public QObject, public HintInterface{
	Q_OBJECT

public:
	ScriptAPI(MainWindow *mainWindow);

	Q_INVOKABLE void addHint(QString const &message, int const duration); ///Message in error reporter
	Q_INVOKABLE void arrowToWidget(QWidget *target, qreal angle); ///Draw arrow near target
	Q_INVOKABLE void moveVirtualCursor(QString const &type, QString const &target, int duration, int xSceneCoord = 0, int ySceneCoord = 0); ///Moves virtual cursor to target with type for duration ms
	Q_INVOKABLE QString dragPaletteElement(QString const &paletteElementId, int const duration, int const xSceneCoord, int const ySceneCoord);
	Q_INVOKABLE void virtualCursorLeftClick(QString const &type, QString const &target);
	Q_INVOKABLE QString createBlockOnScene(DraggableElement const  *paletteElement, int const xSceneCoord, int const ySceneCoord);
	Q_INVOKABLE void drawLink(QString const &fromElementId, QString const &toElementId, int const duration);
	Q_INVOKABLE void open2dModel(QWidget *pluginWidget);
	Q_INVOKABLE void close2dModel();

private slots:
	void destroyHint();
	void virtualDrawing();
	void edgeFinished();

private:
	QScriptEngine mScriptEngine;
	MainWindow *mMainWindow;
	VirtualCursor *mVirtualCursor;
	QPropertyAnimation *mCursorMoveAnimation;
	GuiFacade *mGuiFacade;
	HintReporter *mHintReporter;
	bool mRightButtonPressed;
	QEventLoop mVirtualMouseEventLoop;
};

}
}
