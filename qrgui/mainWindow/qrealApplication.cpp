/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "qrealApplication.h"

#include <QtWidgets/QWidget>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>

#include <qrkernel/settingsManager.h>
#include <qrkernel/logging.h>
#include <qrutils/virtualKeyboard.h>

using namespace qReal;

QRealApplication::QRealApplication(int &argc, char **argv)
	: QApplication(argc, argv)
{
	connect(this, &QRealApplication::focusChanged, this, &QRealApplication::onFocusChanged);
}

bool QRealApplication::notify(QObject *obj, QEvent *e)
{
	/// @todo: Move user actions logging into plugin?
	/// Then we can ask user about his agreement for actions collection in installer.

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

	emit lowLevelEvent(obj, e);
	return QApplication::notify(obj, e);
}

void QRealApplication::logMouse(QWidget * const target, QMouseEvent * const event)
{
	if (!target) {
		return;
	}

	QWidget * const window = target->window();
	const QPoint pos = target->mapTo(window, event->pos());
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
	const QPoint pos = target->mapTo(window, event->pos());
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
	const QPoint pos = target->mapTo(window, event->pos());
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
