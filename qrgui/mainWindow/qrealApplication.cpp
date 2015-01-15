#include "qrealApplication.h"

#include <QtWidgets/QWidget>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>

#include <qrkernel/settingsManager.h>
#include <qrkernel/logging.h>
#include <qrutils/uxInfo/uxInfo.h>
#include <qrutils/virtualKeyboard.h>

using namespace qReal;

QRealApplication::QRealApplication(int &argc, char **argv)
	: QApplication(argc, argv)
{
	connect(this, &QRealApplication::focusChanged, this, &QRealApplication::onFocusChanged);
}

bool QRealApplication::notify(QObject *obj, QEvent *e)
{
	/// @todo: restore UX info reporting.

	switch (e->type()) {
	case QEvent::MouseButtonPress:
	case QEvent::MouseButtonRelease:
		logMouse(dynamic_cast<QWidget*>(obj), static_cast<QMouseEvent *>(e));
		break;
	case QEvent::Wheel:
		logWheel(dynamic_cast<QWidget*>(obj), static_cast<QWheelEvent *>(e));
		break;
	case QEvent::KeyPress:
	case QEvent::KeyRelease:
		logKey(static_cast<QKeyEvent *>(e));
		break;
	case QEvent::Drop:
		logDrop(dynamic_cast<QWidget*>(obj), static_cast<QDropEvent *>(e));
		break;
	default:
		break;
	}

	return QApplication::notify(obj, e);
}

void QRealApplication::logMouse(QWidget * const target, QMouseEvent * const event)
{
	if (!target) {
		return;
	}

	QWidget * const window = target->window();
	QPoint const pos = target->mapTo(window, event->pos());
	QLOG_TRACE() << "Mouse"
			<< (event->type() == QEvent::MouseButtonPress ? "press" : "release")
			<< "in" << pos << "with" << event->button() << "target"
			<< window->windowTitle() << window->size();
}

void QRealApplication::logWheel(QWidget * const target, QWheelEvent * const event)
{
	if (!target) {
		return;
	}

	QWidget * const window = target->window();
	QPoint const pos = target->mapTo(window, event->pos());
	QLOG_TRACE() << "Wheel with delta"
			<< event->angleDelta()
			<< "in" << pos << "target"
			<< window->windowTitle() << window->size();
}

void QRealApplication::logKey(QKeyEvent * const event)
{
	QLOG_TRACE() << "Key"
			<< (event->type() == QEvent::KeyPress ? "press" : "release")
			<< "with" << event->key() << "modifiers" << event->modifiers();
}

void QRealApplication::logDrop(QWidget * const target, QDropEvent * const event)
{
	if (!target) {
		return;
	}

	QWidget * const window = target->window();
	QPoint const pos = target->mapTo(window, event->pos());
	QLOG_TRACE() << "Drop in"
			<< "in" << pos << "with target"
			<< window->windowTitle() << window->size();
}

void QRealApplication::onFocusChanged(QWidget *old, QWidget *now)
{
	/// @todo: when osk.exe is called focus always returns to this process and calls it over and over again;
	/// need to memorize last focused widget.
	if (old != now && SettingsManager::value("touchMode").toBool()
			&& now && now->testAttribute(Qt::WA_InputMethodEnabled)) {
		utils::VirtualKeyboard::show();
	}
}
