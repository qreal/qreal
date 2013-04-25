#include <QtCore/QEvent>
#include <QtGui/QActionEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QKeySequence>

#include "qRealApplication.h"
#include "hotKeyManager/hotKeyManager.h"

using namespace qReal;

QRealApplication::QRealApplication(int &argc, char **argv)
	: QApplication(argc, argv)
{
}


bool QRealApplication::notify(QObject *receiver, QEvent *event)
{
	switch(event->type()) {
		case QEvent::KeyPress:
		case QEvent::KeyRelease: {
			HotKeyManager::setCurrentModifier(QKeySequence(static_cast<QKeyEvent *>(event)->modifiers()).toString());
			break;
		}
		case QEvent::Wheel:
		case QEvent::MouseButtonPress:{
			HotKeyManager::doShortcut(event);
			break;
		}
		default: {
		}
	}
	return QApplication::notify(receiver, event);
}
