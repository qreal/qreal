#pragma once

#include <QtScript/QScriptEngine>

#include "virtualCursor.h"
#include "hintReporter.h"
#include "guiFacade.h"
#include "toolPluginInterface/usedInterfaces/hintInterface.h"

namespace qReal {
namespace gui {

class ScriptAPI : public QObject, public HintInterface{
	Q_OBJECT

public:
	ScriptAPI(MainWindow *mainWindow);

	///Send message as HintReporter.
	Q_INVOKABLE void addHint(QString const &message, int const duration);

	///Draw arrow near target.
	Q_INVOKABLE void arrowToWidget(QWidget *target, qreal angle);

	///Stop execution for duration.
	Q_INVOKABLE void wait(int duration);

	///Start node
	Q_INVOKABLE QString initialNode();

	///Gui facade
	GuiFacade *guiFacade();

public slots:
	void breakWaiting();

private slots:
	void destroyHint();

private:
	QScriptEngine mScriptEngine;
	HintReporter *mHintReporter;
	QEventLoop mEventLoop;
	MainWindow *mMainWindow;
	VirtualCursor *mVirtualCursor;
	GuiFacade *mGuiFacade;
};

}
}
