#pragma once

#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QWidget>
#include "mainwindow/mainWindow.h"


namespace qReal {
namespace gui {

class Arrow : public QWidget{
	Q_OBJECT

public:
	Arrow(QPoint const &sourcePoint, QPoint const &destPoint, MainWindow *mainWindow);

protected:
	void paintEvent(QPaintEvent *);

private:
	QPoint mSourcePoint;
	QPoint mDestPoint;
	qreal mArrowSize;
};

}
}
