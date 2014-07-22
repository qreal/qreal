#include "qrealApplication.h"

#include <QtGui/QMouseEvent>
#include <QtWidgets/QWidget>

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
	case QEvent::KeyPress:
	case QEvent::KeyRelease:
		logKey(static_cast<QKeyEvent *>(e));
		break;
	default:
		break;
	}

	return QApplication::notify(obj, e);
}

void QRealApplication::logMouse(QWidget * const target, QMouseEvent * const event)
{
	if (!target) {
		QLOG_DEBUG() << "Mouse event with unknown target!";
		return;
	}

	QPoint const pos = target->mapTo(target->window(), event->pos());
	QLOG_DEBUG() << "Mouse"
			<< (event->type() == QEvent::MouseButtonPress ? "press" : "release")
			<< "in" << pos << "with" << event->button();
}

void QRealApplication::logKey(QKeyEvent * const event)
{
	QLOG_DEBUG() << "Key"
			<< (event->type() == QEvent::KeyPress ? "press" : "release")
			<< "with" << event->key() << "modifiers" << event->modifiers();
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
