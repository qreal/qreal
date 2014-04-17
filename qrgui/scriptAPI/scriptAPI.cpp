#include "scriptAPI.h"

#include <QtCore/QPropertyAnimation>
#include <QtCore/qmath.h>
#include <QCoreApplication>

#include <qrkernel/exception/exception.h>

#include "virtualCursor.h"
#include "arrow.h"

using namespace qReal;
using namespace gui;

ScriptAPI::ScriptAPI(MainWindow *mainWindow)
	: mScriptEngine(new QScriptEngine)
	, mMainWindow (mainWindow)
	, mHintReporter (new HintReporter(mainWindow))
	, mVirtualCursor (new VirtualCursor(mainWindow))
	, animation (new QPropertyAnimation(mVirtualCursor, "geometry"))
{
	QScriptValue scriptAPI = mScriptEngine.newQObject(this);
	mScriptEngine.globalObject().setProperty("scriptAPI", scriptAPI);

	QString fileName(":/hintScripts/test.js");

	QFile scriptFile(fileName);
	scriptFile.open(QIODevice::ReadOnly);
	QTextStream stream(&scriptFile);
	mScriptEngine.evaluate(stream.readAll(), fileName);

	scriptFile.close();
	moveVirtualCursor(50,50);
}

void ScriptAPI::addHint(QString const &message)
{
	mHintReporter->addHint(message);
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

void ScriptAPI::moveVirtualCursor(int xcoord, int ycoord)
{
//	int targetXCoord = target->pos().x() + target->width()/2;
//	int targetYCoord = target->pos().x() + target->height()/2;


	animation->setDuration(10000); //
	animation->setStartValue(QRect(0, 0, 100, 30));
	animation->setEndValue(QRect(250, 250, 100, 30));

	animation->start();
	//QTest::mouseClick( mMainWindow, Qt::LeftButton, 0, QPoint(250,250));
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), mMainWindow, SLOT(virtualClick()));

	timer->setSingleShot (true);
	timer->start (10000);
}

void ScriptAPI::dragPaletteElement()
{
	//mVirtualCursor->attachPaletteElement();
}

void ScriptAPI::createBlockOnScene()
{
	//mVirtualCursor->attachPaletteElement();
}


