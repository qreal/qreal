#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsScene>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtCore/QString>

#include "mainwindow/gesturesPainterInterface.h"

namespace Ui {
	class GesturesWidget;
}

namespace qReal {
namespace gestures {

class GesturesWidget : public QWidget, public GesturesPainterInterface
{
	Q_OBJECT

public:
	explicit GesturesWidget(QWidget *parent = 0);
	~GesturesWidget();
	virtual void draw(PathVector const &paths);
	virtual Id currentElement() const;
	virtual void setElements(QList<QPair<QString, Id> > const &elements);

signals:
	void currentElementChanged();

private:
	Ui::GesturesWidget *mUi;
	QGraphicsScene *mGestureScene;
	PathVector mPaths;
	QColor gestColor;
	static int const pointsAtSegment = 5;
	int mCurrentPointNumber;
	static int coord(int previous, int next, int part);
	static short const minBoarder = -1000;
	static short const gestWidth = 3;
};

}
}
