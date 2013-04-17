#pragma once

#include <QtWidgets/QAction>

class HotKeyAction : public QAction
{
	Q_OBJECT
public:
	explicit HotKeyAction(QObject *parent = 0);
	
	void press();

signals:
	void pressed();
};
