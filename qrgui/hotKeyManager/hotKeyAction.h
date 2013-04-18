#pragma once

#include <QtWidgets/QAction>
#include <QtCore/QString>
#include <QtCore/QHash>

#include "hotKeyManagerTypes.h"

namespace qReal {

class HotKeyAction : public QAction
{
	Q_OBJECT
public:
	explicit HotKeyAction(QObject *parent = 0);
	
	void press();
	void resetMouseShortcuts();
	void addNewMouseShortcut(QString const keyseq, MouseShortcuts msc);
	QList<QPair<QString, MouseShortcuts> > mouseShortcuts();

signals:
	void pressed();

private:
	QList<QPair<QString, MouseShortcuts> > mMouseShortcuts;
};

}
