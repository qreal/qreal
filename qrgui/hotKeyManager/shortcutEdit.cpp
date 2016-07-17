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

#include "shortcutEdit.h"

#include <QtWidgets/QLineEdit>
#include <QtCore/QEvent>
#include <QtCore/QDebug>
#include <QtGui/QKeyEvent>
#include <QtGui/QKeySequence>

ShortcutEdit::ShortcutEdit(QWidget *parent)
	: QLineEdit(parent)
{
}

bool ShortcutEdit::event(QEvent *event)
{
	if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
		QKeyEvent *keyevent = static_cast<QKeyEvent *> (event);

		switch (keyevent->key()) {
			case Qt::Key_Shift:
			case Qt::Key_Control:
			case Qt::Key_Alt:
			case Qt::Key_AltGr:
				emit newModifiers(keyevent->modifiers());
				break;
			default:
				if (event->type() == QEvent::KeyPress) {
					emit newKey(keyevent->key());
				}
		}
		return true;
	}
	return QLineEdit::event(event);
}
