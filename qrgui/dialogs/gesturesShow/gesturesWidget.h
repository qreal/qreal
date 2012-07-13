#pragma once

#include <QWidget>
#include <QGraphicsScene>
#include <QList>
#include <QPoint>
#include <QString>
#include "../../mainwindow/gesturesPainterInterface.h"

int const minBoarder = -1000;
QColor const gestColor(Qt::blue);
short const gestWidth(3);

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
    PathVector mPaths;
    static const int pointsAtSegment = 5;
    int mCurrentPointNumber;
    static int coord(int previous, int next, int part);
};
