#include "virtualKeyboard.h"

#include <QtTest/QTest>
#include <QtCore/QTimer>

#include "scriptAPI.h"

using namespace qReal;
using namespace gui;

VirtualKeyboard::VirtualKeyboard(ScriptAPI *scriptAPI)
	: mScriptAPI(scriptAPI)
{
}

void VirtualKeyboard::type(QString const &message, int const duration)
{
	printValue(message, duration);
	QTest::keyClick((QWidget *) nullptr, Qt::Key_Enter, Qt::NoModifier, 200);
	mScriptAPI->wait(300);
}

void VirtualKeyboard::printValue(QString const &value, int const duration)
{
	int const charDuration = duration / value.length();
	for (QChar ch : value) {
		QTimer *timer = new QTimer(this);
		timer->setInterval(charDuration);
		timer->setSingleShot(true);
		connect(timer, SIGNAL(timeout()), mScriptAPI, SLOT(breakWaiting()));
		timer->start();
		mScriptAPI->wait(-1);
		QTest::keyClicks(nullptr, ch.toLower(), Qt::NoModifier, 0);
	}
}
