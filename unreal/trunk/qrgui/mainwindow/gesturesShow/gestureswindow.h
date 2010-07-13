#pragma once
#include <QWidget>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>

namespace Ui {
    class GesturesWindow;
}

class GesturesWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GesturesWindow(QWidget *parent = 0);
    ~GesturesWindow();
    QString currentElement();
    void draw(QList<QPoint> const & idealPath);
    void setElements(QList<QString> const & elements);

private:
    Ui::GesturesWindow *ui;
    QTimer *mTimer;
    QGraphicsScene *mGestureScene;
    QList<QPoint> mPath;
    static const int pointsAtSegment = 5;
    int mCurrentPointNumber;
    static int coord(int previous, int next, int part);

private slots:
    void draw();
};

