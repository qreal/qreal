#pragma once

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QList>
#include <QPoint>
#include <QString>
#include "../../mainwindow/gesturesPainterInterface.h"

namespace Ui {
	class GesturesWidget;
}

class GesturesWidget : public QWidget, public GesturesPainterInterface
{
	Q_OBJECT

public:
	explicit GesturesWidget(QWidget *parent = 0);
	~GesturesWidget();
	void draw(PathVector const &paths);
	QString currentElement();
	void setElements(QList<QString> const & elements);

signals:
	void currentElementChanged();

private:
	Ui::GesturesWidget *ui;
	QGraphicsScene *mGestureScene;
	//QTimer *mTimer;
	PathVector mPaths;
	static const int pointsAtSegment = 5;
	int mCurrentPointNumber;
	static int coord(int previous, int next, int part);

private slots:
	//void drawGesture();
};
