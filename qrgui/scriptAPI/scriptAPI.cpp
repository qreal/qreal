#include "scriptAPI.h"

#include <math.h>

#include "arrow.h"
#include <qrkernel/exception/exception.h>

using namespace qReal;
using namespace gui;

ScriptAPI::ScriptAPI(ErrorReporter *errorReporter, MainWindow *mainWindow)
	: mScriptEngine(new QScriptEngine)
	, mErrorReporter(errorReporter)
	, mMainWindow (mainWindow)
{
	QScriptValue scriptAPI = mScriptEngine.newQObject(this);
	mScriptEngine.globalObject().setProperty("scriptAPI", scriptAPI);

	QString fileName(":/hintScripts/test.js");

	QFile scriptFile(fileName);
	scriptFile.open(QIODevice::ReadOnly);
	QTextStream stream(&scriptFile);
	mScriptEngine.evaluate(stream.readAll(), fileName);
	scriptFile.close();

	arrowToWidget();
}

void ScriptAPI::addHint(QString const &message)
{
	mErrorReporter->addHint(message);
}

void ScriptAPI::arrowToWidget()
{
	QPoint one = QPoint(0,0);
	QPoint two = QPoint(100,100);
	Arrow *line = new Arrow(one, two, mMainWindow);
	line->show();
}
