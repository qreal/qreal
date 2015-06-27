#include "virtualKeyboard.h"

#include <QtTest/QTest>
#include <QtCore/QTimer>

#include "scriptAPI.h"

using namespace qReal;
using namespace gui;

VirtualKeyboard::VirtualKeyboard(ScriptAPI &scriptAPI)
	: mScriptAPI(scriptAPI)
{
}

void VirtualKeyboard::type(const QString &message, int duration)
{
	if (QApplication::focusWidget()) {
		printValue(message, duration);
		QTest::keyClick(QApplication::focusWidget(), Qt::Key_Enter, Qt::NoModifier, 0);
		mScriptAPI.wait(300);
	}
}

void VirtualKeyboard::printValue(const QString &value, int duration)
{
	const int charDuration = duration / value.length();
	for (QChar ch : value) {
		QTimer *timer = new QTimer(this);
		timer->setInterval(charDuration);
		timer->setSingleShot(true);
		connect(timer, &QTimer::timeout, &mScriptAPI, &ScriptAPI::breakWaiting);
		timer->start();
		mScriptAPI.wait(-1);
		QTest::keyClicks(nullptr, ch.toLower(), Qt::NoModifier, 0);
	}
}
