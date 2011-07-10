#pragma once

#include <QWidget>
#include <QGraphicsScene>
#include <QTimer>
#include <QList>
#include <QPoint>
#include <QString>
#include "../gesturesPainterInterface.h"

namespace Ui {
	class GesturesWidget;
}

class GesturesWidget : public QWidget, public GesturesPainterInterface
{
	Q_OBJECT

public:
	explicit GesturesWidget(QWidget *parent = 0);
	~GesturesWidget();
	void draw(QList<QPoint> const & path);
	QString currentElement();
	void setElements(QList<QString> const & elements);

signals:
	void currentElementChanged();

private:
	Ui::GesturesWidget *ui;
	QGraphicsScene *mGestureScene;
	QTimer *mTimer;
	QList<QPoint> mPath;
	static const int pointsAtSegment = 5;
	int mCurrentPointNumber;
	static int coord(int previous, int next, int part);

private slots:
	void drawGesture();

};
