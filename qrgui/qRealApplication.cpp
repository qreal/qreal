#include "qRealApplication.h"

#include <QtCore/QEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QWheelEvent>

QRealApplication::QRealApplication(int &argc, char **argv)
	: QApplication(argc, argv)
{
}

bool QRealApplication::event(QEvent *event)
{
	switch (event->type()) {
		case QEvent::KeyPress:
		case QEvent::KeyRelease:
			return true;
		case QEvent::Wheel:
			return true;
		default:
			return QApplication::event(event);
	}
}
