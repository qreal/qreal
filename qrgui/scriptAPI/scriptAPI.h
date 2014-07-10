#pragma once

#include <QtScript/QScriptEngine>
#include <QtCore/QEventLoop>

#include "virtualCursor.h"
#include "hintReporter.h"
#include "guiFacade.h"
#include "toolPluginInterface/usedInterfaces/hintInterface.h"

namespace qReal {
namespace gui {

class ScriptAPI : public QObject, public HintInterface
{
	Q_OBJECT

public:
	ScriptAPI();

	///Initialize script API.
	void init(MainWindow *mainWindow);

	///Send message as HintReporter.
	Q_INVOKABLE void addHint(QString const &message, int const duration);

	///Draw arrow near target.
	Q_INVOKABLE void arrowToWidget(QWidget *target, qreal angle);

	///Stop execution for duration.
	Q_INVOKABLE void wait(int duration);

	///Returns Id of start node.
	Q_INVOKABLE QString initialNode();

	///Change virtual cursor parent.
	Q_INVOKABLE void changeWindow(QWidget *parent);

	///Returns gui facade.
	GuiFacade *guiFacade();

public slots:
	///Break event loop.
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
