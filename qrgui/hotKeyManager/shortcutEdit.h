#pragma once

#include <QtWidgets/QLineEdit>
#include <QtGui/QKeySequence>

class ShortcutEdit : public QLineEdit
{
	Q_OBJECT
public:
	explicit ShortcutEdit(QWidget *parent = 0);

	bool event(QEvent *event);
	
signals:
	void newKey(int const key);
	void newModifiers(Qt::KeyboardModifiers modifiers);
};
