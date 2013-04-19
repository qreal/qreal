#include "qRealApplication.h"
#include "hotKeyManager/hotKeyManager.h"

#include <QtCore/QEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QMouseEvent>
#include <QtCore/QDebug>

using namespace qReal;

QRealApplication::QRealApplication(int &argc, char **argv)
	: QApplication(argc, argv)
{
}

void QRealApplication::nothing()
{
	qDebug() << "ok";
}

bool QRealApplication::notify(QObject *receiver, QEvent *event)
{
	switch(event->type()) {
		case QEvent::KeyPress: {
			HotKeyManager::doShortcut(static_cast<QKeyEvent *> (event));
			return true;
		}
		case QEvent::KeyRelease: {
			HotKeyManager::clearCurrentKeySeq();
			return true;
		}
		case QEvent::Wheel:
			HotKeyManager::doShortcut(static_cast<QWheelEvent *> (event));
			return true;
		case QEvent::MouseButtonPress:
			HotKeyManager::doShortcut(static_cast<QMouseEvent *> (event));
		default: {
			return QApplication::notify(receiver, event);
		}
	}
}
