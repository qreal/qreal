#include "arrow.h"

#include <math.h>
#include <QtCore/qmath.h>

#include <qrkernel/exception/exception.h>

#include "scriptAPI.h"

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
}

void ScriptAPI::addHint(QString const &message)
{
	mErrorReporter->addHint(message);
}

void ScriptAPI::arrowToWidget(QWidget *target, qreal angle)
{
	int const sourcePointDeviation = 50;
	int const intoTargetDeviation = 10;
	QPoint sourcePoint = QPoint(target->x() - sourcePointDeviation * qSin(angle)
						, target->y() + sourcePointDeviation * qCos(angle));
	QPoint destPoint = QPoint(target->x() + intoTargetDeviation,target->y() + intoTargetDeviation);
	Arrow *line = new Arrow(sourcePoint, destPoint, mMainWindow);
	line->show();
}
