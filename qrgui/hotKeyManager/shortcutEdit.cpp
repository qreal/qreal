#include "shortcutEdit.h"
#include "QtWidgets/QLineEdit"
#include <QtCore/QEvent>
#include <QtCore/QDebug>
#include <QtGui/QKeyEvent>


ShortcutEdit::ShortcutEdit(QObject *parent)
	: QLineEdit(static_cast<QWidget *> (parent))
{
}

bool ShortcutEdit::event(QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		qDebug() << static_cast<QKeyEvent *> (event)->key();
		return true;
	}
	return QLineEdit::event(event);
}
