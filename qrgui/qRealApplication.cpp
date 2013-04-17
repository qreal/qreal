#include "qRealApplication.h"
#include "hotKeyManager/hotKeyManager.h"

#include <QtCore/QEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QWheelEvent>
#include <QtCore/QDebug>

using namespace qReal;

QRealApplication::QRealApplication(int &argc, char **argv)
	: QApplication(argc, argv)
{
}


bool QRealApplication::notify(QObject *receiver, QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		HotKeyManager::doShortCut(static_cast<QKeyEvent *> (event));
		return true;
	}
	return QApplication::notify(receiver, event);
}
