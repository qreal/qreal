#pragma once
#include <QGraphicsView>
#include <QKeyEvent>

class View : public QGraphicsView
{
	Q_OBJECT
public:
	View(QWidget *parent = 0);

public slots:
	void zoomIn();
	void zoomOut();

signals:
	void deleteItem();

protected:
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
};
