#include "gestureswindow.h"
#include "ui_gestureswindow.h"

GesturesWindow::GesturesWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GesturesWindow)
{
    ui->setupUi(this);
    mGestureScene = new QGraphicsScene(ui->gvGesture);
    ui->gvGesture->setScene(mGestureScene);
}

void GesturesWindow::draw(QList<QPoint> const & idealPath)
{
    mCurrentPointNumber = 0;
    mGestureScene->clear();
    mPath = idealPath;
    mTimer = new QTimer(ui->gvGesture);
    QObject::connect(mTimer, SIGNAL(timeout()), this, SLOT(draw()));
    mTimer->start();
}

void GesturesWindow::setElements(const QList<QString> &elements)
{
    foreach(QString element, elements)
    {
    }
}

QString GesturesWindow::currentElement()
{
    QString element = "";
    if (ui->lvElements->size() > 0)
    {
        //elements = ui->lvElements->
    }
    return element;
}

void GesturesWindow::draw()
{
    if (mPath.isEmpty())
        return;
    int verticeIndex = (mCurrentPointNumber / pointsAtSegment) % mPath.size();
    int segmentNumber = mCurrentPointNumber % pointsAtSegment + 1;
    if (verticeIndex == mPath.size() - 1)
    {
        mGestureScene->clear();
        mCurrentPointNumber = 0;
        return;
    }
    QPoint lastPaintedPoint = mPath.at(verticeIndex);
    QPoint nextPoint = mPath.at(verticeIndex + 1);
    QPoint currentPoint(coord(lastPaintedPoint.x(), nextPoint.x(), segmentNumber),
                        coord(lastPaintedPoint.y(), nextPoint.y(), segmentNumber));
    mGestureScene->addLine(QLine(lastPaintedPoint, currentPoint), QPen(Qt::black));

    mCurrentPointNumber++;
}

int GesturesWindow::coord(int previous, int next, int part)
{
    return previous + (next - previous) * part / pointsAtSegment;
}

GesturesWindow::~GesturesWindow()
{
    delete ui;
}
